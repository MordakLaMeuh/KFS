//! This file contains some dummy process tests

extern "C" {
    pub static _dummy_asm_process_code: u8;
    pub static _dummy_asm_process_len: usize;

    pub static dummy_c_process: u8;

    pub static kernel_stack: u8;
}