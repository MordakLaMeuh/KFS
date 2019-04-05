use super::standard_sodomizer::make_somization;
use crate::interrupts;
use crate::io::UART_16550;
use crate::math::random::{srand, srand_init};
use crate::memory;
use crate::memory::allocator::kernel_allocator::{kfree, kmalloc, ksize};
use crate::memory::tools::DeviceMap;
use crate::multiboot::MultibootInfo;
use crate::tests::helpers::exit_qemu;

#[no_mangle]
pub extern "C" fn kmain(multiboot_info: *const MultibootInfo, device_map_ptr: *const DeviceMap) -> u32 {
    unsafe {
        UART_16550.init();
    }
    let multiboot_info: MultibootInfo = unsafe { *multiboot_info };
    unsafe {
        interrupts::init();
    }
    crate::watch_dog();
    unsafe {
        let device_map = crate::memory::tools::get_device_map_slice(device_map_ptr);
        memory::init_memory_system(multiboot_info.get_memory_amount_nb_pages(), device_map).unwrap();
    }
    crate::watch_dog();

    srand_init(42).unwrap();
    make_somization(1024, 1000, kmalloc, kfree, ksize, || 4096).expect("failed sodo 0");
    make_somization(1024, 1000, kmalloc, kfree, ksize, || srand::<u32>(16) as usize * 4096).expect("failed sodo 1");
    make_somization(1024, 1000, kmalloc, kfree, ksize, || srand::<u32>(32) as usize * 4096).expect("failed sodo 2");
    make_somization(1024, 1000, kmalloc, kfree, ksize, || srand::<u32>(64) as usize * 4096).expect("failed sodo 3");
    make_somization(1024, 1000 * 4, kmalloc, kfree, ksize, || srand::<u32>(4096) as usize).expect("failed sodo 4");

    crate::watch_dog();
    exit_qemu(0);
    0
}