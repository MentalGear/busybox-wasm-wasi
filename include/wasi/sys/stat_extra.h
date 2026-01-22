/* Extra sys/stat declarations for WASI */
#ifndef _SYS_STAT_EXTRA_H
#define _SYS_STAT_EXTRA_H

#include <sys/types.h>

/* These functions are declared but will return errors at runtime */
int mknod(const char *pathname, mode_t mode, dev_t dev);
int mkfifo(const char *pathname, mode_t mode);

#endif
