use core::mem::size_of;

use std::fs::File as StdFile;
use std::io::{Read, Seek, SeekFrom, Write};

const START_OF_PARTITION: u64 = 0;

/// This newtype handle a pure IO object
/// Must implements 'read', 'write', 'seek', 'flush' and 'try_clone'
#[derive(Debug)]
pub struct ReaderDisk(pub StdFile);

impl ReaderDisk {
    /// Raw read. Fill the buf with readen data on file object
    pub fn read_buffer(&mut self, offset: u32, buf: &mut [u8]) -> usize {
        self.0
            .seek(SeekFrom::Start(offset as u64 + START_OF_PARTITION))
            .expect("seek failed in disk_read_buffer");
        self.0.read(buf).expect("read failed in disk_read_buffer")
    }

    /// Raw write. Write the buf inside file object
    pub fn write_buffer(&mut self, offset: u32, buf: &[u8]) -> usize {
        self.0
            .seek(SeekFrom::Start(offset as u64 + START_OF_PARTITION))
            .expect("seek failed in disk_write_buffer");
        self.0
            .write_all(buf)
            .expect("write failed in disk_write_buffer");
        self.0.flush().expect("flush failed");
        buf.len()
    }

    /// Read a particulary struct in file object
    pub fn read_struct<T: Copy>(&mut self, offset: u32) -> T {
        let t: T;
        unsafe {
            t = core::mem::uninitialized();
            self.read_buffer(
                offset,
                core::slice::from_raw_parts_mut(&t as *const T as *mut u8, size_of::<T>()),
            );
        }
        t
    }

    /// Write a particulary struct inside file object
    pub fn write_struct<T: Copy>(&mut self, offset: u32, t: &T) {
        let s = unsafe { core::slice::from_raw_parts(t as *const _ as *const u8, size_of::<T>()) };
        self.write_buffer(offset, s);
    }

    /// Try to clone xD
    pub fn try_clone(&self) -> std::io::Result<Self> {
        Ok(Self(self.0.try_clone()?))
    }
}
