/* Extra unistd declarations for WASI */
#ifndef _UNISTD_EXTRA_H
#define _UNISTD_EXTRA_H

#include <sys/types.h>

/* These functions are declared but may return errors at runtime */
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);

pid_t fork(void);
pid_t vfork(void);
int execve(const char *pathname, char *const argv[], char *const envp[]);
int execv(const char *pathname, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int setuid(uid_t uid);
int seteuid(uid_t uid);
int setgid(gid_t gid);
int setegid(gid_t gid);
int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);
int getgroups(int size, gid_t list[]);
int setgroups(size_t size, const gid_t *list);

pid_t setsid(void);
pid_t getsid(pid_t pid);
pid_t getpgid(pid_t pid);
int setpgid(pid_t pid, pid_t pgid);
pid_t getpgrp(void);
int setpgrp(void);
pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgrp);

int nice(int inc);
int sethostname(const char *name, size_t len);
int gethostname(char *name, size_t len);
char *getlogin(void);
int getlogin_r(char *buf, size_t bufsize);

int pipe(int pipefd[2]);
int pipe2(int pipefd[2], int flags);
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int dup3(int oldfd, int newfd, int flags);

int chroot(const char *path);

unsigned int alarm(unsigned int seconds);
int pause(void);

#endif

/* Time-related functions that may not be in WASI */
#include <time.h>
int clock_settime(clockid_t clk_id, const struct timespec *tp);
int settimeofday(const struct timeval *tv, const struct timezone *tz);
int adjtime(const struct timeval *delta, struct timeval *olddelta);


/* Temporary file functions */
char *mktemp(char *template);
char *mkdtemp(char *template);
int mkstemp(char *template);
int mkostemp(char *template, int flags);
int mkstemps(char *template, int suffixlen);
int mkostemps(char *template, int suffixlen, int flags);


/* File mode and permission functions */
mode_t umask(mode_t mask);
int fchmod(int fd, mode_t mode);
int lchmod(const char *path, mode_t mode);


/* Directory functions */
int fchdir(int fd);

/* TTY functions */
int ttyname_r(int fd, char *buf, size_t buflen);
char *ttyname(int fd);
int isatty(int fd);


/* Linux-specific file reading functions */
ssize_t readahead(int fd, off_t offset, size_t count);

