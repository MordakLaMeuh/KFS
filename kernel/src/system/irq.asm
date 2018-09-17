[BITS 32]
segment .data

panic_buf: times 512 db 0

divide_by_zero_msg: db "Divide by zero", 0
page_fault_msg: db "Page fault at address %p err_reg: 0x%.8x", 0

segment .text
extern panic
extern sprintk

GLOBAL asm_default_interrupt
GLOBAL asm_divide_by_zero
GLOBAL asm_page_fault
GLOBAL asm_default_pic_master_interrupt
GLOBAL asm_default_pic_slave_interrupt
GLOBAL asm_clock_handler
GLOBAL asm_keyboard_handler

%macro PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET 0
    pushad                ; EAX, ECX, EDX, EBX, and ESP, EBP, ESI, EDI
    push dword [ebp + 16] ; eflags
    push dword [ebp + 12] ; cs
    push dword [ebp + 8]  ; eip
    push ss
    push es
    push ds
%endmacro

%macro PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET 0
    pushad                ; EAX, ECX, EDX, EBX, and ESP, EBP, ESI, EDI
    push dword [ebp + 12] ; eflags
    push dword [ebp + 8]  ; cs
    push dword [ebp + 4]  ; eip
    push ss
    push es
    push ds
%endmacro

%macro POP_ALL_REGISTERS 0
    pop ds
    pop es
    add esp, 16
    popad
%endmacro

asm_default_interrupt:
    iret

; when a normal CPU interruption is launched, EFLAGS, CS and EIP are pushed.
; in the case of page_fault, an other value (err_code) is pushed after.
; see 'rec03-2.pdf' at page 11 for more explanation.
;
; to execute IRET corectly we must add esp by 4 or pop something to skip
; err_code
extern page_fault_handler
asm_page_fault:
    push ebp
    mov ebp, esp

    PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET

; C manager execution, test if this page fault is not fatal
    mov eax, cr2
    push eax
    mov eax, [ebp + 4]
    push eax
    call page_fault_handler
    add esp, 8
    cmp eax, 0
    je .end ; if OKAY, jump to the end

; panic execution block, fill the error string and launch the BSOD
    push dword [ebp + 4]
    mov eax, cr2
    push eax
    push page_fault_msg
    push panic_buf

    call sprintk
    add esp, 16

    push panic_buf
    call panic
; the execution cannot continue here

; end segment, return to programm
.end:
    POP_ALL_REGISTERS

    pop ebp
    ; bypass the error code
    add esp, 4
    iret

extern divide_by_zero_handler
asm_divide_by_zero:
    push ebp
    mov ebp, esp

    PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET

    call divide_by_zero_handler
    cmp eax, 0
    je .end

; panic execution block, fill the error string and launch the BSOD
    push divide_by_zero_msg
    call panic

.end
    POP_ALL_REGISTERS

    pop ebp
    iret

asm_default_pic_master_interrupt:
    mov al, 0x20
    out 0x20, al
    iret

asm_default_pic_slave_interrupt:
; IRQ8 is managed by master and slave, so we must inform the two PICS
    mov al, 0x20
    out 0x20, al
    mov al, 0xA0
    out 0xA0, al
    iret

extern putstr
asm_clock_handler:
	push eax
    mov al, 0x20
    out 0x20, al
	pop eax
    iret

extern process_keyboard

; 8042 chipset
; 60h read or transmit data
; 64h compute status or emmit command
asm_keyboard_handler:
    in al, 0x64
    mov edx, eax
    and edx, 0x1
    cmp edx, 0
    je asm_keyboard_handler ; wait after kerboard buffer is full

    xor eax, eax
    in al, 0x60 ; read scan_code

    push eax
    call process_keyboard
    add esp, 4

    mov al, 0x20
    out 0x20, al
    iret
