; see https://wiki.osdev.org/Paging
; This file defines constants and methods to set kernel in half high memory
; It must be include in all boot/init sources files before switch to half high memory

; Early paging
extern virtual_offset

; Some usefull paging const
%define READ_WRITE (1 << 1)
%define PRESENT (1 << 0)

; Only the first 20 bits is signifiant for pages
%define PAGE_MASK 0xfffff000
%define PAGE_SIZE 4096
%define PAGE_TABLE_PER_DIRECTORY 1024

; Initialize page pointer
%macro INIT_PAGING_ENV 0
	mov edx, page_table_alpha_area
	mov eax, page_pointer
	mov dword [eax], edx
%endmacro

; Convert a size in bytes to a number of pages: fn foo(eax) -> eax
%macro BYTES_TO_PAGES 0
	push edx
	mov edx, eax
	shr eax, 12
	and edx, PAGE_MASK
	cmp edx, 0
	je %%next
	inc eax
%%next:
	pop edx
%endmacro

; Manual pagination system:
; %1 Offset in page directory: value ∈ [0..1024] -> virt_addr ∈ [0x0..0x1_00_00_00_00]
; %2 Physical area associated
; %3 Len of Physical area in mo (BE CAREFULL: MUST BE MULTIPLE OF 2^2)

%macro PAGINATE_ADDR 3
	; EDI = (page_directory_alpha_area - virtual_offset) + (%1 * 4)
	mov edx, %1 * 4
	mov eax, page_directory_alpha_area
	add eax, edx
	mov edi, eax

	mov edx, [page_pointer]

	mov ecx, %3
	shr ecx, 2          ; -> initialize counter of len / 4 (paquets of 4mb
%%write_pd:
	mov eax, edx
	and eax, PAGE_MASK
	or eax, READ_WRITE | PRESENT
	stosd
	add edx, PAGE_SIZE
	loop %%write_pd

	; Mapping of physical address
	mov edi, [page_pointer]

	mov edx, %2                 ; -> beginning of physical area associated
%%write_pages:
	mov eax, edx
	and eax, PAGE_MASK
	or eax, READ_WRITE | PRESENT
	stosd
	add edx, PAGE_SIZE
	cmp edx, %2 + (1 << 20) * %3 ; -> rep until cur_phy_addr != base_phy_addr + len in mb
	jne %%write_pages

	mov dword [page_pointer], edi
%endmacro

; Functionnal pagination system:
; %1 Offset in page directory: value ∈ [0..1024] -> virt_addr ∈ [0x0..0x1_00_00_00_00]
; %2 Physical area associated
; %3 Len of Physical area in octet

; CAUTION: Usable only when high memory is initialized
segment .text
GLOBAL _dynamic_map:
_dynamic_map:
	push ebp
	mov ebp, esp

	push edi
	push ecx
	push edx

	mov eax, [ebp + 8]
	mov edx, 4
	mul edx

	mov edx, eax
	mov eax, page_directory_alpha_area
	add eax, edx
	mov edi, eax

	mov edx, [page_pointer]

	mov eax, [ebp + 16]
	BYTES_TO_PAGES
	mov ecx, eax

.l2_a:
	mov eax, edx
	and eax, PAGE_MASK
	or eax, READ_WRITE | PRESENT
	stosd
	add edx, PAGE_SIZE
	loop .l2_a

	mov edi, [page_pointer]

	mov edx, [ebp + 12]          ; phy_addr
	mov ecx, [ebp + 16]          ; len
	add ecx, edx                 ; phy_addr + len = end_addr

.l2_b:
	mov eax, edx
	and eax, PAGE_MASK
	or eax, READ_WRITE | PRESENT
	stosd
	add edx, PAGE_SIZE
	cmp edx, ecx
	jb .l2_b

	mov dword [page_pointer], edi

	pop edx
	pop ecx
	pop edi

	xor eax, eax

	pop ebp
	ret

segment .data
GLOBAL page_pointer
page_pointer: dd 0xDEADBEEF

segment .bss
align 4096

; 256 ko reserved for alpha pages tables Can allocate 256 mo: Kernel_low: 128 mo. Kernel_High: 128 Mo. Custom, -?-.
; KERNEL SIZE CANNOT EXCEED 128 MO !
GLOBAL page_table_alpha_area
page_table_alpha_area:
	resb 1 << 18

; 4kb reserved for alpha pages table directory: Can allocate 4 go
GLOBAL page_directory_alpha_area
page_directory_alpha_area:
	resb 1 << 12
