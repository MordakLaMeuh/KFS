[BITS 32]

segment .text
align 16

global _idle_process_code
global _idle_process_len

; When it is launched, The idle process `hlt`
_idle_process_code:
	hlt
	jmp _idle_process_code

_idle_process_len:    dd $-_idle_process_code
