#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

bool SIGHANDLER_CALLED = false;

void segv_handler(int signum) {

	printf("segfault handler %i\n", signum);
	exit(0);
}

int main() {
	int *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	*addr = 42;
	printf("write to addr\n");
	int ret = munmap(addr, 4096);
	if (ret == -1) {
		perror("munmap");
		exit(1);
	}
	signal(SIGSEGV, &segv_handler);
	*addr = 45;
	return 1;
}
