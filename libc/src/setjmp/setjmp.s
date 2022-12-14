.intel_syntax noprefix

.set OFF_EBP, 0
.set OFF_EBX, 4
.set OFF_EDI, 8
.set OFF_ESI, 12
.set OFF_ESP, 16
.set OFF_EIP, 20

.section .text

# int setjmp(jmp_buf env) {
.global setjmp
setjmp:
    mov edx, [esp + 4]
    mov eax, [esp] # get eip
    mov [edx + OFF_EIP], eax
    mov [edx + OFF_EBP], ebp
    mov [edx + OFF_EBX], ebx
    mov [edx + OFF_EDI], edi
    mov [edx + OFF_ESI], esi
    mov [edx + OFF_ESP], esp
    xor eax, eax
    ret

# void longjmp(jmp_buf env, int val) {
.global longjmp
longjmp:
	mov edx, [esp + 4] # save env buffer
	mov eax, [esp + 8] # save val return value

	mov esp, [edx + OFF_ESP] # go to new stack frame

	mov ebx, [edx + OFF_EIP]
	mov [esp], ebx           # put eip on the stack

	mov ebp, [edx + OFF_EBP]
	mov ebx, [edx + OFF_EBX]
	mov edi, [edx + OFF_EDI]
	mov esi, [edx + OFF_ESI]
	ret
