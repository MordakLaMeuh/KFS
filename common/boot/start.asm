[BITS 32]

; Initialisation methods prefixed by alt_
extern alt_guard_all
extern alt_clear_screen
extern alt_disable_cursor
extern alt_gdt_new
extern alt_init_early_idt
extern alt_get_device_mem_map
extern alt_init_paging
; extern alt_acpi

%define MULTIBOOT_INFOS_LEN 128

; Declare constants used for creating a multiboot header.
%define MEMINFO   (1 << 0)
%define BOOTDISK  (1 << 1)
%define FLAGS     (MEMINFO | BOOTDISK)
%define MAGIC     0x1BADB002
%define CHECKSUM  - (MAGIC + FLAGS)

; Port of UART
%define UART 0x3f8

; Declare a header as in the Multiboot Standard.
; You don't need to understand all these details as it is just magic values that
; is documented in the multiboot standard. The bootloader will search for this
; magic sequence and recognize us as a multiboot kernel.
; The grub multiboot v1 header MUST be 12 bytes before the _start entry point

; LD script section:
; .boot BLOCK(4K) : ALIGN(4K)
;   {
;       *(*.multiboot)
;   }
;
; OUTPUT elf file:
; desassembly of section .boot:
;
; 00100000 <_start-0xc>:
; 100000:02 b0 ad 1b 03 00    add    0x31bad(%eax),%dh
; 100006:00 00                add    %al,(%eax)
; 100008:fb                   sti
; 100009:4f                   dec    %edi
; 10000a:52                   push   %edx
; 10000b:e4 e9                in     $0xe9,%al
;
; 0010000c <_start>:
; 10000c:e9 ef 0f 00 00       jmp    101000 <init>

extern _init_kernel
segment .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

GLOBAL _start
_start:
	cli
	jmp _init

segment .init
_init:
	; IMPORTANT RULES:
	; - 'Previous EBP' = *'current EBP' -> each EBP's stack frame are pushed into the stack
	; - EIP is always at *(EBP + 4) -> Function name discovery with symbol table
	; - When discovered EIP is 0x0 -> backtracing stopped
	; OPTIONAL:
	; - For backtracking, it is useless to push EBP value at 0xFC
	;
	; 0x100 +----------+----------------------------------------------------------> Temporary stack PTR
	;       |   0x0    | <= mov [temporary_stack - 4], dword 0x0
	; 0xFC  +----------+
	;       | (.ebp..) | Here EBP = 0xF8, so EIP is *(EBP + 4) = *(0xFC) = 0x0
	; 0xF8  +----------+----------------------------------------------------------> ESP / EBP (temporary_stack - 8)
	;       |  ARG 1   |                                  |
	; 0xF4  +----------+         OTHER FUNCTION           |
	;       |   EIP    |            STACK                 |
	; 0xF0  +----------+            FRAME                 |
	;       |   EBP    |                                  |
	; 0xEC  +----------+----------------------------------+
	;       |          |                                  |
	; 0xE8  v          v                                  v

	; Set up the first stack
	mov [temporary_stack - 4], dword 0x0
	mov esp, temporary_stack - 8
	mov ebp, esp

%ifdef EARLY_UART
	call _enable_uart
%endif

%ifdef EARLY_UART
	mov esi, str_multiboot_info
	call _uart_dbg
%endif

	; Store the multiboot info structure pointed by EBX (avoid accidental erasing)
	mov edi, multiboot_infos
	mov esi, ebx
	mov ecx, MULTIBOOT_INFOS_LEN
	cld
	rep movsb

%ifdef EARLY_UART
	mov esi, str_gdt_renew
	call _uart_dbg
%endif

	; Set up a early GDT
	; reserve 8 bytes for structure pointer (need six bytes)
	sub esp, 8
	mov ebx, esp
	push ebx

	call alt_gdt_new

	lgdt [ebx]

	add esp, 8 + 4

	; Set up the Data descriptor segments
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Set up the kernel stack segment
	mov ax, 0x18
	mov ss, ax

	; Set up the code segment
	jmp 0x8: .set_protected_cs
.set_protected_cs:

%ifdef EARLY_UART
	mov esi, str_tss_load
	call _uart_dbg
%endif

	; load the TSS segment
	; Will be used when will switch to ring 0 from ring 3
	mov ax, 0x38
	ltr ax

%ifdef EARLY_UART
	mov esi, str_idt_build
	call _uart_dbg
%endif

	; Set up a early IDT
	; reserve 8 bytes for structure pointer (need six bytes)
	sub esp, 8
	mov ebx, esp
	push ebx

	call alt_init_early_idt

	lidt [ebx]

	add esp, 8 + 4

%ifdef EARLY_UART
	mov esi, str_init_watchdog
	call _uart_dbg
%endif

	; set watchdog
	call alt_guard_all

%ifdef EARLY_UART
	mov esi, str_disable_cursor
	call _uart_dbg
%endif
	call alt_disable_cursor

%ifdef EARLY_UART
	mov esi, str_clear_screen
	call _uart_dbg
%endif
	call alt_clear_screen

	; Do ACPI tests
	; call alt_acpi

	; Get device map in memory and push a pointer to a generated structure
%ifdef EARLY_UART
	mov esi, str_device_map
	call _uart_dbg
%endif
	call alt_get_device_mem_map
	push eax

	; Set up the MMU, prepare switching to high half memory
%ifdef EARLY_UART
	mov esi, str_init_paging
	call _uart_dbg
%endif
	call alt_init_paging

	; Push the grub multiboot header
	push multiboot_infos

	; Call _init_kernel located in high memory !
%ifdef EARLY_UART
	mov esi, str_jump_high_mem
	call _uart_dbg
%endif
	call _init_kernel

	; A long jump can give a adrenaline boost, i dont understand why ...
	; call 0x8:_init_kernel

%ifdef EARLY_UART
_enable_uart:
	push ebp
	mov ebp, esp
	push eax
	push edx

	; disable all interrupts
	mov dx, UART + 1
	xor al, al
	out dx, al

	; enable DLAB (set baud rate divisor
	mov dx, UART + 3
	mov al, 0x80
	out dx, al

	; Set divisor to 3 (lo byte) 38400 baud
	mov dx, UART
	mov al, 0x3
	out dx, al

	; (hi byte)
	mov dx, UART + 1
	xor al, al
	out dx, al

	; 8 bits, no parity, one stop bit
	mov dx, UART + 3
	mov al, 0x3
	out dx, al

	; Enable FIFO, clear them, with 14-byte threshold
	mov dx, UART + 2
	mov al, 0xC7
	out dx, al

	; IRQs enabled, RTS/DSR set
	mov dx, UART + 4
	mov al, 0x1
	out dx, al

	mov esi, str_uart_init
	call _uart_dbg

	pop edx
	pop eax
	pop ebp
	ret

_uart_dbg:
	push ebp
	mov ebp, esp
	pushad

.check_empty:
	mov dx, UART + 5
	in al, dx
	and al, 0x20
	cmp al, 0x0
	je .check_empty

.dump_string:
	lodsb
	cmp al, 0x0
	je .dump_end
	mov dx, UART
	out dx, al
	jmp .dump_string

.dump_end:
	popad
	pop ebp
	ret

str_uart_init: db "Early UART initialized", 10, 0
str_multiboot_info: db "Extracting multiboot info", 10, 0
str_gdt_renew: db "Creating early GDT", 10, 0
str_tss_load: db "Loading TSS", 10, 0
str_idt_build: db "Creating early IDT", 10, 0
str_init_watchdog: db "Initialization of watchdog", 10, 0
str_disable_cursor:	db "Disable cursor", 10, 0
str_clear_screen: db "Clear screen", 10, 0
str_device_map: db "Memory device mapping", 10, 0
str_init_paging: db "Initialization of paging", 10, 0
str_jump_high_mem: db "jump to high memory", 10, 0
%endif

align 16
; 4ko for a temporary stack
times 1 << 12 db 0
temporary_stack:

; Early backup of multiboot info structure
multiboot_infos:
times MULTIBOOT_INFOS_LEN db 0xff
