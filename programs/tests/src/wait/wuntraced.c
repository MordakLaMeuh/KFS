#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid = fork();
	if (pid < 0) {
		printf("fork Failure\n");
	} else if (pid == 0) {
		raise(SIGSTOP);
		printf("end child of life\n");
		while(1) {
			sleep(1);
			printf("I am alive !\n");
			/* sleep(0.01); */
		}
	} else {
		int status;
		pid_t ret = waitpid(pid, &status, WUNTRACED);
		if (ret == -1) {
			perror("waitpid failed 1");
			exit(1);
		}
		printf("raw son status: %hhx   WIFSTOPPED result: %i WIFCONTINUED result: %i\n", status, WIFSTOPPED(status), WIFCONTINUED(status));
		if (!WIFSTOPPED(status)) {
			dprintf(2, "WIFSTOPPED should be true");
			exit(1);
		}
		kill(pid, SIGCONT);
		ret = waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (ret == -1) {
			perror("waitpid failed 2");
			exit(1);
		}
		printf("raw son status: %hhx   WIFSTOPPED result: %i WIFCONTINUED result: %i\n", status, WIFSTOPPED(status), WIFCONTINUED(status));
		if (!WIFCONTINUED(status)) {
			dprintf(2, "WIFCONTINUED should be true");
			exit(1);
		}

		sleep(1);
		kill(pid, SIGKILL);
		printf("I send a SigKill\n");
		if (ret == -1) {
			perror("kill failed");
			exit(1);
		}
		ret = waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (ret == -1) {
			perror("waitpid failed:");
			exit(1);
		}
		printf("raw son status: addr: %p, status: %x WIFSIGNALED: %i\n", &status, status, WIFSIGNALED(status));
		if (!WIFSIGNALED(status)) {
			dprintf(2, "WIFSIGNALED should be true");
			exit(1);
		}
		if (WTERMSIG(status) != SIGKILL) {
			dprintf(2, "WTERMSIG should be SIGKILL");
			exit(1);
		}
		if (WIFEXITED(status)) {
			dprintf(2, "WIFEXITED should be false");
			exit(1);
		}
		printf("raw son status: %hhx   WIFCONTINUED result: %i\n", status, WIFCONTINUED(status));
	}
	return 0;
}
