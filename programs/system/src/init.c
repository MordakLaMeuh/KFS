#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int open_tty_device(const char *tty_device)
{
	int fd = open(tty_device, 0);
	if (fd < 0) {
		exit(1);
	}
	dup(fd);
	dup(fd);
	return fd;
}

int main(int argc, char **argv, char **envp)
{
	pid_t pid = fork();
	if (pid < 0) {
		/* #[allow(unused)] */
		int _fd = open_tty_device("/dev/tty1");
		(void)_fd;
		perror("fork failed");
		exit(1);
	} else if (pid == 0) {
		if (argc < 2) {
			int _fd = open_tty_device("/dev/tty1");
			(void)_fd;
			dprintf(STDERR_FILENO, "ArgC must be at least 2\n");
			exit(1);
		}
		int ret = execve(argv[1], argv + 1, envp);
		if (ret < 0) {
			/* #[allow(unused)] */
			int _fd = open_tty_device("/dev/tty1");
			(void)_fd;
			perror("execve failed");
			exit(1);
		}
	} else {
		while (1) {
			int status;

			pid_t ret = wait(&status);
			if (ret < 0) {
				/* #[allow(unused)] */
				int _fd = open_tty_device("/dev/tty1");
				(void)_fd;
				perror("init wait failed");
				exit(1);
			}
		}
	}
}
