use super::{Driver, FileOperation, IpcResult, SysResult};
use alloc::sync::Arc;
use ext2::Ext2Filesystem;
use libc_binding::{
    blkcnt_t, blksize_t, dev_t, gid_t, ino_t, mode_t, nlink_t, off_t, stat, time_t, timespec,
    uid_t, Whence,
};
use sync::DeadMutex;

/// a driver of an ext2 file
#[derive(Debug)]
pub struct Ext2DriverFile {
    ext2: Arc<DeadMutex<Ext2Filesystem>>,
    inode_nbr: u32,
}

impl Ext2DriverFile {
    pub fn new(ext2: Arc<DeadMutex<Ext2Filesystem>>, inode_nbr: u32) -> Self {
        Self { ext2, inode_nbr }
    }
}

impl Driver for Ext2DriverFile {
    fn open(&mut self) -> SysResult<IpcResult<Arc<DeadMutex<dyn FileOperation>>>> {
        Ok(IpcResult::Done(Arc::new(DeadMutex::new(
            Ext2FileOperation::new(self.ext2.clone(), self.inode_nbr),
        ))))
    }
}

/// a file operation of an ext2 file
#[derive(Debug)]
pub struct Ext2FileOperation {
    ext2: Arc<DeadMutex<Ext2Filesystem>>,
    inode_nbr: u32,
    offset: u64,
}

impl Ext2FileOperation {
    fn new(ext2: Arc<DeadMutex<Ext2Filesystem>>, inode_nbr: u32) -> Self {
        Self {
            ext2,
            inode_nbr,
            offset: 0,
        }
    }
}

impl FileOperation for Ext2FileOperation {
    fn read(&mut self, buf: &mut [u8]) -> SysResult<IpcResult<u32>> {
        let res = self
            .ext2
            .lock()
            .new_read(self.inode_nbr, &mut self.offset, buf)? as u32;
        Ok(IpcResult::Done(res))
    }

    fn fstat(&mut self, stat: &mut stat) -> SysResult<u32> {
        let inode = self.ext2.lock().read_inode(self.inode_nbr)?;

        *stat = stat {
            st_dev: 42 as dev_t,             // Device ID of device containing file.
            st_ino: self.inode_nbr as ino_t, // File serial number.
            st_mode: inode.type_and_perm.bits() as mode_t, // Mode of file (see below).
            st_nlink: inode.nbr_hard_links as nlink_t, // Number of hard links to the file.
            st_uid: inode.user_id as uid_t,  // User ID of file.
            st_gid: inode.group_id as gid_t, // Group ID of file.
            st_rdev: 0 as dev_t, //TODO // Device ID (if file is character or block special).
            st_size: inode.get_size() as off_t, // For regular files, the file size in bytes.
            st_atim: timespec {
                // Last data access timestamp.
                tv_sec: inode.last_access_time as time_t,
                tv_nsec: 0,
            },
            st_mtim: timespec {
                tv_sec: inode.last_modification_time as time_t,
                tv_nsec: 0,
            }, // Last data modification timestamp.
            st_ctim: timespec {
                tv_sec: inode.last_access_time as time_t,
                tv_nsec: 0,
            }, // Last file status change timestamp.
            st_blksize: self.ext2.lock().get_block_size() as blksize_t, // A file system-specific preferred I/O block size
            st_blocks: inode.nbr_disk_sectors as blkcnt_t, // Number of blocks allocated for this object.
        };
        Ok(0)
    }

    fn write(&mut self, _buf: &[u8]) -> SysResult<IpcResult<u32>> {
        unimplemented!();
    }

    fn lseek(&mut self, _offset: off_t, _whence: Whence) -> SysResult<off_t> {
        unimplemented!();
    }
}