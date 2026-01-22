/* sys/wait.h stub for WASI */
#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/types.h>

/* Wait macros - stubbed for WASI */
#define WIFEXITED(status)    (((status) & 0x7f) == 0)
#define WEXITSTATUS(status)  (((status) & 0xff00) >> 8)
#define WIFSIGNALED(status)  (((status) & 0x7f) != 0 && ((status) & 0x7f) != 0x7f)
#define WTERMSIG(status)     ((status) & 0x7f)
#define WIFSTOPPED(status)   (((status) & 0xff) == 0x7f)
#define WSTOPSIG(status)     (((status) & 0xff00) >> 8)
#define WIFCONTINUED(status) ((status) == 0xffff)
#define WCOREDUMP(status)    ((status) & 0x80)

#define WNOHANG     1
#define WUNTRACED   2

/* Stub functions - declared but not necessarily available */
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
pid_t wait3(int *status, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);

#endif /* _SYS_WAIT_H */
