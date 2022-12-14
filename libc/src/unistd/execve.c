#include <ltrace.h>
#include <user_syscall.h>
#include <stdio.h>
#include <errno.h>

/*
 * Execute program
 */
int execve(const char *filename, char *const argv[], char *const envp[])
{
	TRACE
	int ret = _user_syscall(EXECVE, 3, filename, argv, envp);
	/*
	 * On success, execve() does not return, on error -1 is returned, and errno is set appropriately.
	 */
	set_errno_and_return(ret);
}
