use super::virtual_page_allocator::{VirtualPageAllocator, KERNEL_VIRTUAL_PAGE_ALLOCATOR};
use super::{KERNEL_VIRTUAL_MEMORY, KERNEL_VIRTUAL_OFFSET};
use crate::memory::mmu::_enable_paging;
use crate::memory::mmu::{PageDirectory, PAGE_TABLES};
use crate::memory::tools::*;
use crate::memory::BuddyAllocator;
use crate::memory::VIRTUAL_OFFSET;
use alloc::boxed::Box;
use alloc::vec;
use core::alloc::{GlobalAlloc, Layout};

/// 4 MB for the bootstrap
const MEMORY_BOOTSTRAP_KERNEL_ALLOCATOR: usize = 0x400_000;

pub static mut KERNEL_ALLOCATOR: KernelAllocator = KernelAllocator::Bootstrap(BootstrapKernelAllocator::new());

pub enum KernelAllocator {
    Bootstrap(BootstrapKernelAllocator),
    Kernel(SlabAllocator),
}

static mut BSS_MEMORY: [u8; MEMORY_BOOTSTRAP_KERNEL_ALLOCATOR] = [0; MEMORY_BOOTSTRAP_KERNEL_ALLOCATOR];

#[derive(Debug)]
pub struct BootstrapKernelAllocator {
    current_offset: usize,
}

impl BootstrapKernelAllocator {
    pub const fn new() -> Self {
        BootstrapKernelAllocator { current_offset: 0 }
    }
    pub unsafe fn alloc_bootstrap(&mut self, layout: Layout) -> Result<VirtualAddr, MemoryError> {
        let base_address = &BSS_MEMORY[0] as *const u8 as usize;
        let mut address = VirtualAddr(&BSS_MEMORY[self.current_offset] as *const u8 as usize);
        address = address.align_on(layout.align());
        self.current_offset = address.0 - base_address + layout.size();
        if self.current_offset > BSS_MEMORY.len() {
            panic!("No more bootstrap memory");
        }
        Ok(address)
    }
}

pub struct SlabAllocator;

impl SlabAllocator {
    pub unsafe fn alloc(&mut self, size: usize) -> Result<VirtualAddr, MemoryError> {
        KERNEL_VIRTUAL_PAGE_ALLOCATOR.as_mut().unwrap().alloc(size)
    }

    /// size in bytes
    pub unsafe fn free(&mut self, vaddr: VirtualAddr, size: usize) -> Result<(), MemoryError> {
        KERNEL_VIRTUAL_PAGE_ALLOCATOR.as_mut().unwrap().free(vaddr, size)
    }
}

pub struct RustGlobalAlloc;

unsafe impl GlobalAlloc for RustGlobalAlloc {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        match &mut KERNEL_ALLOCATOR {
            KernelAllocator::Kernel(a) => a.alloc(layout.size()).unwrap_or(VirtualAddr(0x0)).0 as *mut u8,
            KernelAllocator::Bootstrap(b) => b.alloc_bootstrap(layout).unwrap_or(VirtualAddr(0x0)).0 as *mut u8,
        }
    }

    unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
        match &mut KERNEL_ALLOCATOR {
            KernelAllocator::Kernel(a) => a.free(VirtualAddr(ptr as usize), layout.size()).unwrap(),
            KernelAllocator::Bootstrap(_) => panic!("try to free while in bootstrap allocator"),
        }
    }
}

#[alloc_error_handler]
#[cfg(not(test))]
fn out_of_memory(_: core::alloc::Layout) -> ! {
    panic!("Out of memory: Failed to allocate a rust data structure");
}

pub unsafe fn init_kernel_virtual_allocator() {
    let buddy = BuddyAllocator::new(
        VirtualAddr(KERNEL_VIRTUAL_OFFSET),
        KERNEL_VIRTUAL_MEMORY,
        vec![0; BuddyAllocator::<VirtualAddr>::metadata_size(KERNEL_VIRTUAL_MEMORY)],
    );
    let mut pd = Box::new(PageDirectory::new());
    pd.set_page_tables(0, &PAGE_TABLES);
    pd.map_range_page_init(VirtualAddr(0).into(), PhysicalAddr(0).into(), NbrPages::_64MB).unwrap();
    pd.map_range_page_init(VirtualAddr(0xc0000000).into(), PhysicalAddr(0x00000000).into(), NbrPages::_64MB).unwrap();
    let raw_pd = Box::into_raw(pd);
    let real_pd = PhysicalAddr(raw_pd as usize - VIRTUAL_OFFSET);
    _enable_paging(real_pd);
    pd = Box::from_raw(raw_pd);
    pd.self_map_tricks(real_pd);
    let virt = VirtualPageAllocator::new(buddy, pd);
    KERNEL_VIRTUAL_PAGE_ALLOCATOR = Some(virt);
    KERNEL_ALLOCATOR = KernelAllocator::Kernel(SlabAllocator);
}