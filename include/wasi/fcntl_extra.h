/* fcntl_extra.h - Extra fcntl constants for WASI */
#ifndef _FCNTL_EXTRA_H
#define _FCNTL_EXTRA_H

#include <fcntl.h>

/* Only define what's missing from WASI SDK's fcntl.h */
#ifndef F_DUPFD
#define F_DUPFD         0
#endif

#ifndef F_GETLK
#define F_GETLK         5
#endif

#ifndef F_SETLK
#define F_SETLK         6
#endif

#ifndef F_SETLKW
#define F_SETLKW        7
#endif

#ifndef F_SETOWN
#define F_SETOWN        8
#endif

#ifndef F_GETOWN
#define F_GETOWN        9
#endif

/* fcntl file locking */
#ifndef F_RDLCK
#define F_RDLCK         0
#endif

#ifndef F_WRLCK
#define F_WRLCK         1
#endif

#ifndef F_UNLCK
#define F_UNLCK         2
#endif

#ifndef F_DUPFD_CLOEXEC
#define F_DUPFD_CLOEXEC 1030
#endif

#endif /* _FCNTL_EXTRA_H */
