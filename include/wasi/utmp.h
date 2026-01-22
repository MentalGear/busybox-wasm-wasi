/* utmp.h stub for WASI */
#ifndef _UTMP_H
#define _UTMP_H

#include <sys/types.h>
#include <sys/time.h>

#define UT_LINESIZE 32
#define UT_NAMESIZE 32
#define UT_HOSTSIZE 256

struct utmp {
    short ut_type;
    pid_t ut_pid;
    char ut_line[UT_LINESIZE];
    char ut_id[4];
    char ut_user[UT_NAMESIZE];
    char ut_host[UT_HOSTSIZE];
    struct {
        int e_termination;
        int e_exit;
    } ut_exit;
    long ut_session;
    struct timeval ut_tv;
    int ut_addr_v6[4];
    char __unused[20];
};

#define EMPTY         0
#define RUN_LVL       1
#define BOOT_TIME     2
#define NEW_TIME      3
#define OLD_TIME      4
#define INIT_PROCESS  5
#define LOGIN_PROCESS 6
#define USER_PROCESS  7
#define DEAD_PROCESS  8
#define ACCOUNTING    9

void setutent(void);
void endutent(void);
struct utmp *getutent(void);
struct utmp *getutid(const struct utmp *ut);
struct utmp *getutline(const struct utmp *ut);
struct utmp *pututline(const struct utmp *ut);
int utmpname(const char *file);

#endif /* _UTMP_H */
