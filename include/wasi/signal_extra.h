/* Extra signal definitions for WASI */
#ifndef _SIGNAL_EXTRA_H
#define _SIGNAL_EXTRA_H

#include <signal.h>
#include <sys/types.h>

/* siginfo_t if not already defined */
#ifndef __siginfo_t_defined
#define __siginfo_t_defined
typedef struct {
    int si_signo;
    int si_errno;
    int si_code;
    pid_t si_pid;
    uid_t si_uid;
    void *si_addr;
    int si_status;
    long si_band;
    union {
        int _pad[32];
        struct {
            pid_t _pid;
            uid_t _uid;
        } _kill;
    } _sifields;
} siginfo_t;
#endif

/* Signal set manipulation macros */
#ifndef SIG_BLOCK
#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
#endif

/* Signal set functions */
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);

/* sigaction structure if not defined by WASI's emulated signal */
#ifndef SA_RESTART
#define SA_RESTART   0x10000000
#define SA_NOCLDSTOP 0x00000001
#define SA_NOCLDWAIT 0x00000002
#define SA_SIGINFO   0x00000004
#define SA_NODEFER   0x40000000
#define SA_RESETHAND 0x80000000

typedef void (*__sighandler_t)(int);

struct sigaction {
    union {
        __sighandler_t sa_handler;
        void (*sa_sigaction)(int, siginfo_t *, void *);
    } __sigaction_handler;
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};
#define sa_handler   __sigaction_handler.sa_handler
#define sa_sigaction __sigaction_handler.sa_sigaction

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigsuspend(const sigset_t *mask);
int sigpending(sigset_t *set);
int sigwait(const sigset_t *set, int *sig);
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
int sigwaitinfo(const sigset_t *set, siginfo_t *info);

int kill(pid_t pid, int sig);
int killpg(pid_t pgrp, int sig);

#endif /* SA_RESTART */

#endif /* _SIGNAL_EXTRA_H */
