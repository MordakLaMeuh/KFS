#include <ltrace.h>
#include <user_syscall.h>
#include <wait.h>
#include <errno.h>

/*
 * Each of these calls sets errno to an appropriate value in the case of an error
 */

/*
 * wait for process to change state
 */
pid_t waitpid(pid_t pid, int *wstatus, int options)
{
	TRACE
	pid_t ret = _user_syscall(WAITPID, 3, pid, wstatus, options);
	/*
	 * on success, returns the process ID of the child whose state
	 * has changed; if WNOHANG was specified and one or more child(ren)
	 * specified by pid exist, but have not yet changed state, then 0
	 * is returned.  On error, -1 is returned.
	 */

	set_errno_and_return(ret);
}

/*
 * wait for process to change state
 */
pid_t wait(int *wstatus)
{
	TRACE
	/*
	 * on success, returns the process ID of the terminated child;
	 * on error, -1 is returned.
	 */
	return waitpid(-1, wstatus, 0);
}
