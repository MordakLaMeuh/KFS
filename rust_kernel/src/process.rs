#[cfg_attr(rustfmt, rustfmt_skip)]
struct Tss {
    /*0x00*/ _reserved1: u16, link: u16,
    /*0x04*/ esp0: u32,
    /*0x08*/ _reserved2: u16, ss0: u16,
    /*0x0C*/ esp1: u32,
    /*0x10*/ _reserved3: u16, ss1: u16,
    /*0x14*/ esp2: u32,
    /*0x18*/ _reserved4: u16, ss2: u16,
    /*0x1C*/ cr3: u32,
    /*0x20*/ eip: u32,
    /*0x24*/ eflags: u32,
    /*0x28*/ eax: u32,
    /*0x2C*/ ecx: u32,
    /*0x30*/ edx: u32,
    /*0x34*/ ebx: u32,
    /*0x38*/ esp: u32,
    /*0x3C*/ ebp: u32,
    /*0x40*/ esi: u32,
    /*0x44*/ edi: u32,
    /*0x48*/ _reserved5: u16, es: u16,
    /*0x4C*/ _reserved6: u16, cs: u16,
    /*0x50*/ _reserved7: u16, ss: u16,
    /*0x54*/ _reserved8: u16, ds: u16,
    /*0x58*/ _reserved9: u16, fs: u16,
    /*0x5C*/ _reserved10: u16, gs: u16,
    /*0x60*/ _reserved11: u16, ldtr: u16,
}

extern "C" {
    pub fn load_tss(tss_gdt_descriptor: u8);
}
