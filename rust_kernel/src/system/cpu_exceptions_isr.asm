[BITS 32]

; https://wiki.osdev.org/Exceptions
; These CPU ISR gates are on vector 0 -> 31

%macro PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET 0
	pushad

	mov eax, [ebp]        ; get stored ebp
	mov [esp + 8], eax

	mov eax, [ebp + 20]   ; esp
	mov [esp + 12], eax
	push dword [ebp + 16] ; eflags
	push dword [ebp + 12] ; cs
	push dword [ebp + 8]  ; eip

	; SS and ESP are present only when we came from a greater ring level
	; Don't trust them in others cases
	mov ax, [ebp + 24]    ; ss
	and eax, 0xffff
	push eax

	xor eax, eax
	mov [esp - 4], eax
	push gs
	mov [esp - 4], eax
	push fs
	mov [esp - 4], eax
	push es
	mov [esp - 4], eax
	push ds
%endmacro

%macro PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET 0
	pushad

	mov eax, [ebp]        ; get stored ebp
	mov [esp + 8], eax

	mov eax, [ebp + 16]   ; esp
	mov [esp + 12], eax
	push dword [ebp + 12] ; eflags
	push dword [ebp + 8]  ; cs
	push dword [ebp + 4]  ; eip

	; SS and ESP are present only when we came from a greater ring level
	; Don't trust them in others cases
	mov ax, [ebp + 20]    ; ss
	and eax, 0xffff
	push eax

	xor eax, eax
	mov [esp - 4], eax
	push gs
	mov [esp - 4], eax
	push fs
	mov [esp - 4], eax
	push es
	mov [esp - 4], eax
	push ds
%endmacro

extern cpu_panic_handler

%macro CREATE_ISR 3
segment .data
	isr_%1_str: db %2, " error", 0
segment .text
GLOBAL _isr_%1
_isr_%1:
	push ebp
	mov ebp, esp
	%3
	push isr_%1_str
	call cpu_panic_handler
%endmacro

extern cpu_page_fault_handler

segment .text
	GLOBAL _isr_page_fault
_isr_page_fault:
	push ebp
	mov ebp, esp
	; push all the purposes registers
	PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
	; push the error code
	mov eax, [ebp + 4]
	push eax
	; push the fault address
	mov eax, cr2
	push eax
	call cpu_page_fault_handler
	add esp, 32 + 4 + 4; to be on the pushad
	popad
	pop ebp
	add esp, 4 ; to be on the eip
	iret

; After expansion of macro (for cpu_default_interrupt)
; segment .data
; isr_cpu_default_interrupt_str:
;     db "cpu default interrupt", 0
; segment .text
; GLOBAL _isr_cpu_default_interrupt
; _isr_cpu_default_interrupt:
;     push ebp
;     mov ebp, esp
; # MACRO BLOC: PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
;     pushad                ; EAX, ECX, EDX, EBX, and ESP, EBP, ESI, EDI
;     push dword [ebp + 12] ; eflags
;     push dword [ebp + 8]  ; cs
;     push dword [ebp + 4]  ; eip
;     push ss
;     push gs
;     push fs
;     push es
;     push ds
; # END MACRO BLOC
;     push isr_cpu_default_interrupt_str
;     call panic

; CPU default interrupt without err_code
CREATE_ISR cpu_default_interrupt, "cpu default interrupt", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET

; CPU ISR without err_code
CREATE_ISR debug, "debug", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR divide_by_zero, "division by zero", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR non_maskable_interrupt, "non maskable interrupt", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR breakpoint, "breakpoint", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR overflow, "overflow", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR bound_range_exceeded, "bound range exceeded", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR invalid_opcode, "invalid_opcode", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR no_device, "no device", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR fpu_seg_overrun, "fpu seg overrun", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR fpu_floating_point_exep, "fpu floating point exeption", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR machine_check, "machine check", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR simd_fpu_fp_exception, "simd fpu fp exception", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET
CREATE_ISR virtualize_exception, "virtualize exception", PUSH_ALL_REGISTERS_WITHOUT_ERRCODE_OFFSET

; CPU ISR with err_code
CREATE_ISR double_fault, "double fault", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR invalid_tss, "invalid tss", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR seg_no_present, "segment no present", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR stack_seg_fault, "stack segment fault", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR general_protect_fault, "general protection fault", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR alignment_check, "alignment check", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
CREATE_ISR security_exception, "security exception", PUSH_ALL_REGISTERS_WITH_ERRCODE_OFFSET
