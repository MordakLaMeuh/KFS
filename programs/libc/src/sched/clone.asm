%define CLONE 120

global sys_clone
sys_clone:
	push ebp
	mov ebp, esp

	push ebx
	push ecx
	mov eax, CLONE
	mov ebx, [ebp + 8]
	mov ecx, [ebp + 12]
	int 80h
	cmp eax, 0
	je clone_child
	pop ecx
	pop ebx

	pop ebp
	ret


extern continue_clone_child

clone_child:
	call continue_clone_child
	
