#ifndef __PWD_H__
# define __PWD_H__

//The <pwd.h> header shall define the struct passwd, structure, which shall include at least the following members:

#include <sys/types.h>

struct passwd {
	char    *pw_name ; //  User's login name.
	uid_t    pw_uid  ; //  Numerical user ID.
	gid_t    pw_gid  ; //  Numerical group ID.
	char    *pw_dir  ; //  Initial working directory.
	char    *pw_shell; //  Program to use as shell.
};

//The <pwd.h> header shall define the gid_t, uid_t, and size_t types as described in <sys/types.h>.

//The following shall be declared as functions and may also be defined as macros. Function prototypes shall be provided.

//[XSI][Option Start]
void           endpwent(void);
struct passwd *getpwent(void);
//[Option End]
struct passwd *getpwnam(const char *);
int            getpwnam_r(const char *, struct passwd *, char *,
                   size_t, struct passwd **);
struct passwd *getpwuid(uid_t);
int            getpwuid_r(uid_t, struct passwd *, char *,
                   size_t, struct passwd **);
//[XSI][Option Start]
void           setpwent(void);
//[Option End]

#endif