/* sys/sysmacros.h stub for WASI */
#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H

#define major(dev)  ((unsigned int)(((dev) >> 8) & 0xff))
#define minor(dev)  ((unsigned int)((dev) & 0xff))
#define makedev(ma, mi)  ((unsigned int)(((ma) << 8) | (mi)))

#endif /* _SYS_SYSMACROS_H */
