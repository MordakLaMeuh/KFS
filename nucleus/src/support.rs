#[lang = "eh_personality"]
extern "C" fn eh_personality() {
}
/*#[lang = "panic_fmt"]
fn panic_fmt() -> ! {
    loop {}
}

#[lang = "begin_unwind"]
pub extern "C" fn begin_unwind() {
}*/
use core::panic::PanicInfo;
#[panic_handler]
#[no_mangle]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[no_mangle]
pub unsafe extern "C" fn memcpy(dest: *mut u8,
                                src: *const u8,
                                n: usize)
                                -> *mut u8 {
    let mut i = 0;
    while i < n {
        *dest.offset(i as isize) = *src.offset(i as isize);
        i += 1;
    }
    dest
}

#[no_mangle]
pub unsafe extern "C" fn memset(s: *mut u8, c: i16, n: usize) -> *mut u8 {
    let mut i = 0;
    while i < n {
        *s.offset(i as isize) = c as u8;
        i += 1;
    }
    s
}
