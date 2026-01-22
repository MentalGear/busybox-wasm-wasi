/* wasix_compat.h - Compatibility declarations for WASIX builds
 *
 * WASIX libc doesn't provide some POSIX functions that BusyBox needs.
 * This header provides declarations for stub implementations in wasi_stubs.c.
 */

#ifndef WASIX_COMPAT_H
#define WASIX_COMPAT_H

#include <sys/types.h>

/* mknod - WASIX doesn't have device node creation */
int mknod(const char *pathname, mode_t mode, dev_t dev);

/* chown family - WASIX doesn't have ownership management */
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);

/* gethostname/sethostname */
int gethostname(char *name, size_t len);
int sethostname(const char *name, size_t len);

/* getlogin */
char *getlogin(void);
int getlogin_r(char *buf, size_t bufsize);

/* chroot */
int chroot(const char *path);

/* readahead */
ssize_t readahead(int fd, off_t offset, size_t count);

/* getgroups - WASIX doesn't have this */
int getgroups(int size, gid_t list[]);

/* getuid/getgid - WASIX libc provides: geteuid, getegid, setuid, setgid, seteuid, setegid
 * We only need to declare getuid, getgid, setreuid, setregid */
uid_t getuid(void);
gid_t getgid(void);
int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);

/* fchdir - WASIX doesn't have this */
int fchdir(int fd);

/* settimeofday - WASIX doesn't have this */
struct timeval;
int settimeofday(const struct timeval *tv, const void *tz);

#endif /* WASIX_COMPAT_H */
