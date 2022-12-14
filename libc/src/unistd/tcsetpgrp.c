#include <ltrace.h>
#include <user_syscall.h>
#include <unistd.h>
#include <errno.h>

// If the process has a controlling terminal, tcsetpgrp() shall set
// the foreground process group ID associated with the terminal to
// pgid_id. The application shall ensure that the file associated with
// fildes is the controlling terminal of the calling process and the
// controlling terminal is currently associated with the session of
// the calling process. The application shall ensure that the value of
// pgid_id matches a process group ID of a process in the same session
// as the calling process.

// Attempts to use tcsetpgrp() from a process which is a member of a
// background process group on a fildes associated with its
// controlling terminal shall cause the process group to be sent a
// SIGTTOU signal. If the calling thread is blocking SIGTTOU signals
// or the process is ignoring SIGTTOU signals, the process shall be
// allowed to perform the operation, and no signal is sent.

int tcsetpgrp(int fildes, pid_t pgid_id) {
	int ret = _user_syscall(TCSETPGRP, 2, fildes, pgid_id);
	set_errno_and_return(ret);
}
