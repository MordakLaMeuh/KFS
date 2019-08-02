#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <custom.h>

struct program_test {
	char *path;
	/* char **argv; */
};

static struct program_test TEST_PROGRAMS[] = {
	{.path = "/bin/SignalSimple"},
	{.path = "/bin/SignalSimpleDuo"},
	{.path = "/bin/ProcessGroup"}
};

#define TEST_PROGRAMS_LEN sizeof(TEST_PROGRAMS) / sizeof(struct program_test)

int main() {
	for (unsigned int i = 0; i < TEST_PROGRAMS_LEN; i++) {
		char *env[] = {NULL};
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork failed");
			exit(1);
		} else if (pid == 0) {
			char *args[2] = {
				TEST_PROGRAMS[i].path,
				NULL,
			};
			pid_t child_pid = getpid();
			printf("child_pid: %d\n", child_pid);
			execve(TEST_PROGRAMS[i].path, args, env);
			perror("execve failed");
			exit_qemu(1);
		} else {
			pid_t father_pid = getpid();
			printf("father_pid: %d\n", father_pid);
			printf("I am the father, i wait my child\n");
			int status;
			int ret = wait(&status);
			if (ret == -1) {
				perror("wait failed");
				exit_qemu(1);
			}
			if (status != 0) {
				// qemu exit fail
				printf("test: '%s' failed", TEST_PROGRAMS[i].path);
				printf("status '%d'", status);
				sleep(1000);
				if (!WIFEXITED(status)) {
					exit_qemu(1);
				}
				if (WEXITSTATUS(status) != 0) {
					exit_qemu(1);
				}
			}
		}
	}
	exit_qemu(0);
}
