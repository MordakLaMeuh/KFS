PATH  := $(PATH):/toolchain_turbofish/cross/bin/
SHELL := env PATH=$(PATH) /bin/bash

TARGET = libc.a
SYSROOT = /toolchain_turbofish/sysroot

AS = i686-turbofish-as
CC = i686-turbofish-gcc
AR = i686-turbofish-ar
RANLIB = i686-turbofish-ranlib

# non-turbofish specific program. Use carefully
NASM = nasm

IFLAGS = -Iinclude
HEADERS = include/i386.h

VPATH += src
SRC_C += libc_ctor
SRC_ASM += user_syscall
HEADERS += include/user_syscall.h

SRC_C += memset bzero memcpy memccpy memmove memchr memcmp \
	strlen strcpy strncpy strcat strncat strlcat \
	strchr strrchr strstr strnstr strcmp strncmp \
	atoi toupper \
	strclr striter striteri \
	strequ strnequ \
	putendl putnbr putnbr_base \
	aligned_memcpy aligned_bzero strdup \
	strcasecmp strcspn strpbrk strspn strtod strtok strtoll strtoull strchrnul strerror \
	strcoll
VPATH += src/string
HEADERS += include/string.h

SRC_C += beacon op_main modifiers extract_args \
	s_string s_char s_numeric_u s_numeric_i \
	s_pointer s_logical_b s_logical_o s_logical_xmin s_logical_xmaj	\
	cast buffer norme perror \
	stdio _doprnt putchar puts getc fopen
VPATH += src/stdio/printf src/stdio/
HEADERS += include/stdio.h internal_printf.h

SRC_C += ctor mem_syscall \
	size_fn \
	index \
	debug_show_alloc debug_display_nodes \
	node_custom_allocator node_custom_deallocator \
	cmp_range cmp_fixe \
	free_record \
	allocator \
	reallocator \
	deallocator \
	main_prototypes \
	trace \
	sizeof_object \
	alloc_btree_ctor alloc_btree_get_node alloc_btree_apply_infix alloc_btree_memory_move alloc_btree_atomics_op \
	alloc_btree_insert_rnb_node alloc_btree_delete_rnb_node alloc_btree_insert_strategy alloc_btree_rotation_node \
	alloc_btree_try_to_insert alloc_btree_family_node alloc_btree_get_last_valid alloc_btree_delete_node \
	alloc_btree_insert_node alloc_btree_swap_content alloc_btree_limit_items alloc_btree_delete_strategy \
	alloc_btree_get_neighbours
VPATH += src/stdlib/alloc src/stdlib/alloc/alloc_btree
HEADERS += include/stdlib.h main_headers.h alloc_btree_internal_header.h

SRC_C += exit exit_qemu atexit getenv abort abs qsort _Exit
VPATH += src/stdlib
HEADERS += include/stdlib.h

SRC_C += read write fork mmap munmap mprotect sleep getuid getpid close unlink pause reboot shutdown execve getpgid getpgrp setpgid getppid tcsetpgrp tcgetpgrp tcgetpgrp tcsetpgrp isatty getegid geteuid getgid chdir getcwd setegid seteuid setgid setgroups setuid seteuid setegid setgroups getgroups _exit lseek
SRC_C += dup dup2 pipe

VPATH += src/unistd
HEADERS += include/unistd.h

SRC_C += mmap munmap mprotect
VPATH += src/sys/mman 
HEADERS += include/sys/mman.h

SRC_C += stat lstat fstat stat64 mkfifo umask
VPATH += src/sys/stat 
HEADERS += include/sys/stat.h

SRC_C += gettimeofday settimeofday
VPATH += src/sys/time
HEADERS += include/sys/time.h

SRC_ASM += clone
SRC_C += sched
VPATH += src/sched
HEADERS += include/sched.h

SRC_C += nanosleep localtime time gmtime
VPATH += src/time
HEADERS += include/time.h

SRC_C += signal sigaction kill raise killpg \
		sigemptyset sigfillset sigaddset sigdelset \
		strsignal \
        sigprocmask sigsuspend sigismember

VPATH += src/signal
HEADERS += include/signal.h

SRC_C += wait
VPATH += src/wait
HEADERS += include/wait.h

SRC_C += socket bind connect listen accept send recv sendto recvfrom shutdown
VPATH += src/socket
HEADERS += include/sys/socket.h include/sys/un.h

SRC_C += gettext locale
VPATH += src/locale
HEADERS += include/locale.h

SRC_C += tcgetattr tcsetattr
VPATH += src/termios
HEADERS += include/termios.h

SRC_C += opendir readdir closedir
VPATH += src/dirent
HEADERS += include/dirent.h

SRC_C += fcntl open
VPATH += src/fcntl
HEADERS += include/fcntl.h

SRC_C += isdigit isspace isblank iscntrl isgraph islower ispunct isupper isxdigit isalpha isalnum isascii isprint _tolower tolower
VPATH += src/ctype
HEADERS += include/ctype.h

VPATH += src/math
SRC_ASM += pow.c
HEADERS += include/math.h

VPATH += src/setjmp
SRC_ASM += setjmp
HEADERS += include/setjmp.h

VPATH += src/math
SRC_NASM += round
HEADERS += include/math.h

SRC_C += strsplit array parse_2d_file
VPATH += src/tools
HEADERS += include/tools.h

SRC_C += getpwnam getpwuid
VPATH += src/pwd
HEADERS += include/pwd.h

SRC_C += getgrnam getgrgid
VPATH += src/grp
HEADERS += include/grp.h

# CFLAGS += -Wall -Wextra -masm=intel $(IFLAGS) -nostdlib -fno-builtin -fno-stack-protector -nodefaultlibs -fno-exceptions #-nostdinc #$(IFLAGS)
# CFLAGS += -Wall -Wextra -masm=intel $(IFLAGS) -nostdlib -nodefaultlibs -fno-exceptions #-nostdinc #$(IFLAGS)
CFLAGS += -O2 -Wall -Wextra -fno-omit-frame-pointer -masm=intel $(IFLAGS)

#ifeq ($(DEBUG),yes)
#	CFLAGS += -m32 -std=gnu99 -Wextra -Wall -g -O0
#	ASFLAGS += -g
#else ifeq ($(OPTIM),yes)
#	# -fno-omit-frame-pointer keep good call trace for panic()
#	CFLAGS += -std=gnu99 -Wextra -Wall -O3 -fno-omit-frame-pointer
#else
#	CFLAGS += -v -std=gnu99 -Wextra -Wall
#endif

OBJ_DIR = obj
OBJ_ASM = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRC_ASM)))))
OBJ_C = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRC_C)))))

# non-turbofish specific
OBJ_NASM = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRC_NASM)))))

SRC_C_RUNTIME = crt0.s crti.s crtn.s crtbegin.s crtend.s
OBJ_C_RUNTIME = $(addprefix $(OBJ_DIR)/crt/, $(addsuffix .o, $(basename $(notdir $(SRC_C_RUNTIME)))))

.PHONY = all clean fclean re exec

all: $(TARGET) $(OBJ_C_RUNTIME)

$(TARGET): $(OBJ_NASM) $(OBJ_ASM) $(OBJ_C)
	$(AR) rc $@ $^
	$(RANLIB) $@

$(OBJ_DIR)/%.o: %.c Makefile $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.s Makefile $(HEADERS)
	$(AS) -o $@ $<

$(OBJ_DIR)/%.o: %.asm Makefile $(HEADERS)
	$(NASM) -f elf -o $@ $<

# Compile all the C runtime object files
obj/crt/%.o: src/crt/%.s
	$(AS) -o $@ $<

clean:
	find $(OBJ_DIR) -type f -name '*.o' -exec rm -fv {} \;

fclean: clean
	rm -fv $(TARGET)

re: fclean all

exec: all
	./$(TARGET)

install:
	cp libc.a $(SYSROOT)/usr/lib/.
	cp -r include/* $(SYSROOT)/usr/include/
	cp $(OBJ_C_RUNTIME) $(SYSROOT)/usr/lib/.