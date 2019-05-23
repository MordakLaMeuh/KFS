use crate::memory::tools::Virt;
use bitflags::bitflags;

#[allow(dead_code)]
// void *mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);
fn sys_mmap(_addr: Virt, _length: usize, _prot: MmapProt, _flags: MmapFlags, _fd: i32, _pgoffset: usize) -> i32 {
    -1
}

bitflags! {
    struct MmapProt: u32 {
        /// Pages may be executed.
        const EXEC  = 1;
        /// Pages may be read.
        const READ  = 1 << 1;
        ///Pages may be written.
        const WRITE  = 1 << 2;
        ///Pages may not be accessed.
        const NONE  = 1 << 3;

    }
}

bitflags! {
    struct MmapFlags: u32 {
        /// Share this mapping.  Updates to the mapping are visible to other
        /// processes mapping the same region, and (in  the  case  of  file-
        /// backed  mappings)  are  carried  through to the underlying file.
        /// (To precisely control when updates are carried  through  to  the
        /// underlying file requires the use of msync(2).)
        const MAP_SHARED = 0;

        /// Create  a private copy-on-write mapping.  Updates to the mapping
        /// are not visible to other processes mapping the  same  file,  and
        /// are  not carried through to the underlying file.  It is unspeci‐
        /// fied whether changes made to the file after the mmap() call  are
        /// visible in the mapped region.
        const MAP_PRIVATE = 0;

        /// Put  the  mapping  into  the  first  2  Gigabytes of the process
        /// address space.  This flag  is  supported  only  on  x86-64,  for
        /// 64-bit  programs.   It  was  added  to allow thread stacks to be
        /// allocated somewhere in the  first  2 GB  of  memory,  so  as  to
        /// improve  context-switch performance on some early 64-bit proces‐
        /// sors.  Modern x86-64 processors no longer have this  performance
        /// problem,  so  use of this flag is not required on those systems.
        /// The MAP_32BIT flag is ignored when MAP_FIXED is set.
        const MAP_32BIT = 0;

        /// Synonym for MAP_ANONYMOUS.  Deprecated.
        const MAP_ANON = 0;

        /// The mapping is not backed by any file; its contents are initial‐
        /// ized  to zero.  The fd argument is ignored; however, some imple‐
        /// mentations require fd to be -1 if MAP_ANONYMOUS (or MAP_ANON) is
        /// specified,  and  portable  applications should ensure this.  The
        /// offset argument should be zero.  The  use  of  MAP_ANONYMOUS  in
        /// conjunction  with  MAP_SHARED  is  supported on Linux only since
        /// kernel 2.4.
        const MAP_ANONYMOUS = 0;

        /// This flag is ignored.  (Long ago, it signaled that  attempts  to
        /// write  to  the  underlying  file should fail with ETXTBUSY.  But
        /// this was a source of denial-of-service attacks.)
        const MAP_DENYWRITE = 0;

        /// This flag is ignored.
        const MAP_EXECUTABLE = 0;

        /// Compatibility flag.  Ignored.
        const MAP_FILE = 0;

        /// Don't interpret addr as a hint = place  the  mapping  at  exactly
        /// that address.  addr must be suitably aligned = for most architec‐
        /// tures a multiple of the page size is sufficient;  however,  some
        /// architectures may impose additional restrictions.  If the memory
        /// region specified by addr and len overlaps pages of any  existing
        /// mapping(s),  then the overlapped part of the existing mapping(s)
        /// will be discarded.  If the specified  address  cannot  be  used,
        /// mmap()  will  fail.  Software that aspires to be portable should
        /// use this option with care, keeping in mind that the exact layout
        /// of  a  process's  memory  mappings is allowed to change signifi‐
        /// cantly between kernel versions, C library versions, and  operat‐
        /// ing system releases.
        /// For example, thread A looks through /proc/<pid>/maps and locates
        /// an  available  address  range,  while  thread  B  simultaneously
        /// acquires part or all of that same address range.  Thread A  then
        /// calls  mmap(MAP_FIXED), effectively overwriting the mapping that
        /// thread B created.
        const MAP_FIXED = 0;

        /// This flag is used for stacks.  It indicates to the  kernel  vir‐
        /// tual  memory  system  that the mapping should extend downward in
        /// memory.  The return address is one page lower  than  the  memory
        /// area  that  is actually created in the process's virtual address
        /// space.  Touching an address in the "guard" page below  the  map‐
        /// ping  will cause the mapping to grow by a page.  This growth can
        /// be repeated until the mapping grows to within a page of the high
        /// end  of  the  next  lower  mapping,  at which point touching the
        /// "guard" page will result in a SIGSEGV signal.
        const MAP_GROWSDOWN = 0;

        /// Allocate the mapping using "huge pages."  See the  Linux  kernel
        /// source  file Documentation/vm/hugetlbpage.txt for further infor‐
        /// mation, as well as NOTES, below.
        const MAP_HUGETLB = 0;

        /// Used in  conjunction  with  MAP_HUGETLB  to  select  alternative
        /// hugetlb page sizes (respectively, 2 MB and 1 GB) on systems that
        /// support multiple hugetlb page sizes.
        const MAP_HUGE_2MB  = 0;
        const MAP_HUGE_1GB = 0;

        const MAP_LOCKED = 0;

        /// This  flag  is meaningful only in conjunction with MAP_POPULATE.
        /// Don't perform read-ahead = create page tables  entries  only  for
        /// pages that are already present in RAM.  Since Linux 2.6.23, this
        /// flag causes MAP_POPULATE to do nothing.  One day,  the  combina‐
        /// tion of MAP_POPULATE and MAP_NONBLOCK may be reimplemented.
        const MAP_NONBLOCK = 0;

        /// Do  not reserve swap space for this mapping.  When swap space is
        /// reserved, one has the guarantee that it is  possible  to  modify
        /// the  mapping.   When  swap  space  is not reserved one might get
        /// SIGSEGV upon a write if no physical memory  is  available.   See
        /// also  the  discussion of the file /proc/sys/vm/overcommit_memory
        /// in proc(5).  In kernels before 2.6, this flag  had  effect  only
        /// for private writable mappings.
        const MAP_NORESERVE = 0;

        /// Populate  (prefault) page tables for a mapping.  For a file map‐
        /// ping, this causes read-ahead on the file.   This  will  help  to
        /// reduce blocking on page faults later.  MAP_POPULATE is supported
        /// for private mappings only since Linux 2.6.23.
        const MAP_POPULATE = 0;

        /// Allocate the mapping at an address suitable  for  a  process  or
        /// thread  stack.   This  flag is currently a no-op, but is used in
        /// the glibc threading implementation so that if some architectures
        /// require  special  treatment  for  stack allocations, support can
        /// later be transparently implemented for glibc.
        const MAP_STACK = 0;

        /// Don't clear anonymous pages.  This flag is intended  to  improve
        /// performance  on  embedded devices.  This flag is honored only if
        /// the kernel was configured with the  CONFIG_MMAP_ALLOW_UNINITIAL‐
        /// IZED  option.  Because of the security implications, that option
        /// is normally enabled only  on  embedded  devices  (i.e.,  devices
        /// where one has complete control of the contents of user memory).
        const MAP_UNINITIALIZED = 0;
    }
}