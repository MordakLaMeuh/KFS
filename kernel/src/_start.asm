[BITS 32]
section .text
        ; GRUB multiboot spec
        align 4
        dd 0x1BADB002               ; magic
        dd 0x0                      ; flags
        dd - (0x1BADB002 + 0x0)     ; checksum. m+f+c should be zero

extern kmain
extern init_GDT

global _start
_start:
    cli                             ; block interrupts

    mov esp, stack_space            ; set stack pointer

	push 0x0
    call init_GDT

    mov ax, 0x20
    mov ss, ax
    mov esp, 0x300000

    call kmain
.halt:
    hlt                             ; halt the CPU until next interrupt
    jmp .halt

section .bss
resb 8192                           ; 8KB for stack
stack_space: