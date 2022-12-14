#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int fildes = open("/dev/sda1", O_RDONLY);

	if (fildes == -1) {
		exit(1);
	}

	// we look for the magic of the mbr at byte 510
	int ret = lseek(fildes, 1080, SEEK_SET);
	if (ret == -1) {
		perror("lseek");
		exit(1);
	}
	if (ret != 1080) {
		dprintf(2, "lseek should return the offset from the begin of the file");
		exit(1);
	}

	int magic = 0;
	int res = read(fildes, &magic, 2);
	if (res == -1) {
		perror("read");
		exit(1);
	}

	printf("magic %d\n", magic);
	if (magic != 0xef53) {
		dprintf(2, "bad magic on the ext2");
		exit(1);
	}
}
