/* wasi_stubs.c - Stub implementations for POSIX functions not available in WASI
 *
 * These provide minimal functionality to allow busybox to compile and run
 * for basic operations. Many functions return errors or do nothing.
 */

#include "libbb.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/* File descriptor operations */
int dup(int oldfd)
{
    /* WASI doesn't have dup, try to emulate with fcntl */
    return fcntl(oldfd, F_DUPFD, 0);
}

int dup2(int oldfd, int newfd)
{
    if (oldfd == newfd) {
        /* Check if oldfd is valid */
        if (fcntl(oldfd, F_GETFD) == -1)
            return -1;
        return newfd;
    }
    close(newfd);
    return fcntl(oldfd, F_DUPFD, newfd);
}

int dup3(int oldfd, int newfd, int flags)
{
    (void)flags;
    return dup2(oldfd, newfd);
}

int pipe(int pipefd[2])
{
    errno = ENOSYS;
    return -1;
}

int pipe2(int pipefd[2], int flags)
{
    (void)flags;
    errno = ENOSYS;
    return -1;
}

/* Process/user ID functions */
pid_t fork(void)
{
    errno = ENOSYS;
    return -1;
}

pid_t vfork(void)
{
    errno = ENOSYS;
    return -1;
}

int execve(const char *pathname, char *const argv[], char *const envp[])
{
    (void)pathname;
    (void)argv;
    (void)envp;
    errno = ENOSYS;
    return -1;
}

int execv(const char *pathname, char *const argv[])
{
    return execve(pathname, argv, environ);
}

int execvp(const char *file, char *const argv[])
{
    return execve(file, argv, environ);
}

int execvpe(const char *file, char *const argv[], char *const envp[])
{
    return execve(file, argv, envp);
}

pid_t getpid(void)
{
    return 1;  /* Pretend PID 1 for WASI */
}

pid_t getppid(void)
{
    return 0;  /* Pretend parent is kernel/init */
}

uid_t getuid(void)
{
    return 0;  /* Pretend root for WASI */
}

gid_t getgid(void)
{
    return 0;  /* Pretend root group for WASI */
}

uid_t geteuid(void)
{
    return getuid();
}

gid_t getegid(void)
{
    return getgid();
}

int setuid(uid_t uid)
{
    (void)uid;
    return 0;  /* Pretend success */
}

int seteuid(uid_t uid)
{
    (void)uid;
    return 0;
}

int setgid(gid_t gid)
{
    (void)gid;
    return 0;
}

int setegid(gid_t gid)
{
    (void)gid;
    return 0;
}

int setreuid(uid_t ruid, uid_t euid)
{
    (void)ruid;
    (void)euid;
    return 0;
}

int setregid(gid_t rgid, gid_t egid)
{
    (void)rgid;
    (void)egid;
    return 0;
}

int getgroups(int size, gid_t list[])
{
    if (size > 0 && list) {
        list[0] = getgid();
        return 1;
    }
    return 0;
}

int setgroups(size_t size, const gid_t *list)
{
    (void)size;
    (void)list;
    return 0;
}

/* Session/process group functions */
pid_t setsid(void)
{
    return getpid();
}

pid_t getsid(pid_t pid)
{
    (void)pid;
    return getpid();
}

pid_t getpgid(pid_t pid)
{
    (void)pid;
    return getpid();
}

int setpgid(pid_t pid, pid_t pgid)
{
    (void)pid;
    (void)pgid;
    return 0;
}

pid_t getpgrp(void)
{
    return getpid();
}

int setpgrp(void)
{
    return 0;
}

pid_t tcgetpgrp(int fd)
{
    (void)fd;
    return getpid();
}

int tcsetpgrp(int fd, pid_t pgrp)
{
    (void)fd;
    (void)pgrp;
    return 0;
}

/* Misc functions */
int nice(int inc)
{
    (void)inc;
    return 0;
}

int sethostname(const char *name, size_t len)
{
    (void)name;
    (void)len;
    errno = EPERM;
    return -1;
}

int gethostname(char *name, size_t len)
{
    const char *hostname = "wasi";
    if (len > 0) {
        strncpy(name, hostname, len);
        name[len - 1] = '\0';
    }
    return 0;
}

char *getlogin(void)
{
    static char login[] = "wasi";
    return login;
}

int getlogin_r(char *buf, size_t bufsize)
{
    const char *login = "wasi";
    if (bufsize > strlen(login)) {
        strcpy(buf, login);
        return 0;
    }
    return ERANGE;
}

int chroot(const char *path)
{
    (void)path;
    errno = EPERM;
    return -1;
}

unsigned int alarm(unsigned int seconds)
{
    (void)seconds;
    return 0;
}

int pause(void)
{
    errno = EINTR;
    return -1;
}

/* Time functions */
int clock_settime(clockid_t clk_id, const struct timespec *tp)
{
    (void)clk_id;
    (void)tp;
    errno = EPERM;
    return -1;
}

/* settimeofday is declared in unistd_extra.h with forward-declared timezone
 * Don't redefine it here to avoid type conflicts */

int adjtime(const struct timeval *delta, struct timeval *olddelta)
{
    (void)delta;
    if (olddelta) {
        olddelta->tv_sec = 0;
        olddelta->tv_usec = 0;
    }
    return 0;
}

/* File mode functions */
mode_t umask(mode_t mask)
{
    static mode_t current_mask = 022;
    mode_t old = current_mask;
    current_mask = mask & 0777;
    return old;
}

/* Ownership functions - stub since WASI doesn't really support these */
int chown(const char *pathname, uid_t owner, gid_t group)
{
    (void)pathname;
    (void)owner;
    (void)group;
    return 0;  /* Pretend success */
}

int fchown(int fd, uid_t owner, gid_t group)
{
    (void)fd;
    (void)owner;
    (void)group;
    return 0;
}

int lchown(const char *pathname, uid_t owner, gid_t group)
{
    (void)pathname;
    (void)owner;
    (void)group;
    return 0;
}

int lchmod(const char *path, mode_t mode)
{
    (void)path;
    (void)mode;
    return 0;
}

/* mknod */
int mknod(const char *pathname, mode_t mode, dev_t dev)
{
    (void)pathname;
    (void)mode;
    (void)dev;
    errno = EPERM;
    return -1;
}

/* Directory functions */
int fchdir(int fd)
{
    (void)fd;
    errno = ENOSYS;
    return -1;
}

/* TTY functions */
int ttyname_r(int fd, char *buf, size_t buflen)
{
    (void)fd;
    if (buflen > 12) {
        strcpy(buf, "/dev/console");
        return 0;
    }
    return ERANGE;
}

char *ttyname(int fd)
{
    static char buf[16];
    if (ttyname_r(fd, buf, sizeof(buf)) == 0)
        return buf;
    return NULL;
}

/* readahead - just a hint, can return success */
ssize_t readahead(int fd, off_t offset, size_t count)
{
    (void)fd;
    (void)offset;
    (void)count;
    return 0;
}

/* Termios functions */
#include "wasi/termios.h"

int tcgetattr(int fd, struct termios *termios_p)
{
    (void)fd;
    if (termios_p) {
        memset(termios_p, 0, sizeof(*termios_p));
        /* Set some reasonable defaults */
        termios_p->c_lflag = ECHO | ICANON | ISIG;
        termios_p->c_iflag = ICRNL;
        termios_p->c_oflag = OPOST | ONLCR;
        termios_p->c_cflag = CS8 | CREAD;
    }
    return 0;
}

int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
{
    (void)fd;
    (void)optional_actions;
    (void)termios_p;
    return 0;
}

int tcsendbreak(int fd, int duration)
{
    (void)fd;
    (void)duration;
    return 0;
}

int tcdrain(int fd)
{
    (void)fd;
    return 0;
}

int tcflush(int fd, int queue_selector)
{
    (void)fd;
    (void)queue_selector;
    return 0;
}

int tcflow(int fd, int action)
{
    (void)fd;
    (void)action;
    return 0;
}

void cfmakeraw(struct termios *termios_p)
{
    if (termios_p) {
        termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        termios_p->c_oflag &= ~OPOST;
        termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        termios_p->c_cflag &= ~(CSIZE | PARENB);
        termios_p->c_cflag |= CS8;
    }
}

speed_t cfgetispeed(const struct termios *termios_p)
{
    return termios_p ? termios_p->c_ispeed : B9600;
}

speed_t cfgetospeed(const struct termios *termios_p)
{
    return termios_p ? termios_p->c_ospeed : B9600;
}

int cfsetispeed(struct termios *termios_p, speed_t speed)
{
    if (termios_p) termios_p->c_ispeed = speed;
    return 0;
}

int cfsetospeed(struct termios *termios_p, speed_t speed)
{
    if (termios_p) termios_p->c_ospeed = speed;
    return 0;
}

/* ioctl is already declared in sys/ioctl.h with different signature */

/* Signal handling stubs */
#include "wasi/signal_extra.h"

/* WASI SDK defines SIG_IGN and SIG_ERR as function pointers to these */
void __SIG_IGN(int sig) { (void)sig; }
void __SIG_ERR(int sig) { (void)sig; }

/* Process times */
#include <sys/times.h>

clock_t times(struct tms *buf)
{
    if (buf) {
        memset(buf, 0, sizeof(*buf));
    }
    return 0;
}

/* String for signal number */
char *strsignal(int sig)
{
    static char buf[32];
    snprintf(buf, sizeof(buf), "Signal %d", sig);
    return buf;
}

/* Resource limits */
#include "wasi/sys/resource.h"

int getrlimit(int resource, struct rlimit *rlim)
{
    (void)resource;
    if (rlim) {
        rlim->rlim_cur = RLIM_INFINITY;
        rlim->rlim_max = RLIM_INFINITY;
    }
    return 0;
}

int setrlimit(int resource, const struct rlimit *rlim)
{
    (void)resource;
    (void)rlim;
    return 0;
}

int getrusage(int who, struct rusage *usage)
{
    (void)who;
    if (usage) {
        memset(usage, 0, sizeof(*usage));
    }
    return 0;
}

int sigemptyset(sigset_t *set)
{
    if (set) *set = 0;
    return 0;
}

int sigfillset(sigset_t *set)
{
    if (set) *set = ~(sigset_t)0;
    return 0;
}

int sigaddset(sigset_t *set, int signum)
{
    if (set && signum > 0 && signum < 64) {
        *set |= ((sigset_t)1 << signum);
    }
    return 0;
}

int sigdelset(sigset_t *set, int signum)
{
    if (set && signum > 0 && signum < 64) {
        *set &= ~((sigset_t)1 << signum);
    }
    return 0;
}

int sigismember(const sigset_t *set, int signum)
{
    if (set && signum > 0 && signum < 64) {
        return (*set & ((sigset_t)1 << signum)) != 0;
    }
    return 0;
}

/* signal() - store handlers but don't actually do anything with them */
static __sighandler_t sig_handlers[64];

__sighandler_t signal(int signum, __sighandler_t handler)
{
    __sighandler_t old = SIG_DFL;
    if (signum > 0 && signum < 64) {
        old = sig_handlers[signum];
        sig_handlers[signum] = handler;
    }
    return old;
}

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    (void)signum;
    (void)act;
    if (oldact) {
        memset(oldact, 0, sizeof(*oldact));
        oldact->sa_handler = SIG_DFL;
    }
    return 0;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    (void)how;
    (void)set;
    if (oldset) {
        sigemptyset(oldset);
    }
    return 0;
}

int sigsuspend(const sigset_t *mask)
{
    (void)mask;
    errno = EINTR;
    return -1;
}

int sigpending(sigset_t *set)
{
    if (set) {
        sigemptyset(set);
    }
    return 0;
}

int sigwait(const sigset_t *set, int *sig)
{
    (void)set;
    (void)sig;
    errno = EINTR;
    return -1;
}

int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout)
{
    (void)set;
    (void)info;
    (void)timeout;
    errno = EAGAIN;
    return -1;
}

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
    (void)set;
    (void)info;
    errno = EINTR;
    return -1;
}

int raise(int sig)
{
    /* For WASI, raising a signal doesn't do much */
    (void)sig;
    return 0;
}

int kill(pid_t pid, int sig)
{
    (void)pid;
    (void)sig;
    if (pid == getpid() && sig == 0)
        return 0;  /* Check if process exists */
    errno = EPERM;
    return -1;
}

int killpg(pid_t pgrp, int sig)
{
    (void)pgrp;
    (void)sig;
    errno = EPERM;
    return -1;
}

/* Wait functions - WASI doesn't have process management */
pid_t wait(int *status)
{
    (void)status;
    errno = ECHILD;
    return -1;
}

pid_t waitpid(pid_t pid, int *status, int options)
{
    (void)pid;
    (void)status;
    (void)options;
    errno = ECHILD;
    return -1;
}

/* wait3 and wait4 declared in sys/wait.h with struct rusage* - don't redefine */

/* Temp file functions */
static unsigned int temp_counter = 0;

int mkstemp(char *template)
{
    char *p = template + strlen(template) - 6;
    if (p < template || strcmp(p, "XXXXXX") != 0) {
        errno = EINVAL;
        return -1;
    }
    /* Replace XXXXXX with unique suffix */
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    /* Create the file */
    return open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
}

int mkostemp(char *template, int flags)
{
    char *p = template + strlen(template) - 6;
    if (p < template || strcmp(p, "XXXXXX") != 0) {
        errno = EINVAL;
        return -1;
    }
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    return open(template, O_RDWR | O_CREAT | O_EXCL | flags, 0600);
}

int mkstemps(char *template, int suffixlen)
{
    int len = strlen(template);
    char *p = template + len - 6 - suffixlen;
    if (p < template || strncmp(p, "XXXXXX", 6) != 0) {
        errno = EINVAL;
        return -1;
    }
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    return open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
}

int mkostemps(char *template, int suffixlen, int flags)
{
    int len = strlen(template);
    char *p = template + len - 6 - suffixlen;
    if (p < template || strncmp(p, "XXXXXX", 6) != 0) {
        errno = EINVAL;
        return -1;
    }
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    return open(template, O_RDWR | O_CREAT | O_EXCL | flags, 0600);
}

char *mktemp(char *template)
{
    char *p = template + strlen(template) - 6;
    if (p < template || strcmp(p, "XXXXXX") != 0) {
        *template = '\0';
        errno = EINVAL;
        return template;
    }
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    return template;
}

char *mkdtemp(char *template)
{
    char *p = template + strlen(template) - 6;
    if (p < template || strcmp(p, "XXXXXX") != 0) {
        errno = EINVAL;
        return NULL;
    }
    temp_counter++;
    snprintf(p, 7, "%06u", temp_counter % 1000000);
    if (mkdir(template, 0700) != 0) {
        return NULL;
    }
    return template;
}
