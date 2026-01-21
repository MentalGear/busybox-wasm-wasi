/* grp.h stub for WASI */
#ifndef _GRP_H
#define _GRP_H

#include <sys/types.h>

struct group {
    char *gr_name;
    char *gr_passwd;
    gid_t gr_gid;
    char **gr_mem;
};

struct group *getgrnam(const char *name);
struct group *getgrgid(gid_t gid);
int getgrnam_r(const char *name, struct group *grp, char *buf, size_t buflen, struct group **result);
int getgrgid_r(gid_t gid, struct group *grp, char *buf, size_t buflen, struct group **result);
void setgrent(void);
void endgrent(void);
struct group *getgrent(void);
int getgrouplist(const char *user, gid_t group, gid_t *groups, int *ngroups);

#endif /* _GRP_H */
