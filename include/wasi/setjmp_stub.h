/* setjmp_stub.h - Stub setjmp/longjmp for WASI without exceptions
 *
 * This provides minimal stubs that allow compilation but will abort
 * if actually called. Only use for utilities that don't need setjmp.
 *
 * Must be included BEFORE any system headers to prevent setjmp.h inclusion.
 */
#ifndef _WASI_SETJMP_STUB_H
#define _WASI_SETJMP_STUB_H

/* Prevent system setjmp.h from being included */
#define _SETJMP_H 1
#define __wasilibc___setjmp_h 1

#include <stdlib.h>

/* Minimal jmp_buf - matches WASI SDK structure size */
typedef struct {
    unsigned long __jb[8];
} __jmp_buf_tag;

typedef __jmp_buf_tag jmp_buf[1];
typedef __jmp_buf_tag sigjmp_buf[1];

/* setjmp always returns 0 (normal path) - defined as inline function */
static inline int __stub_setjmp(jmp_buf env) {
    (void)env;
    return 0;
}

static inline int __stub_sigsetjmp(sigjmp_buf env, int savemask) {
    (void)env;
    (void)savemask;
    return 0;
}

#define setjmp(env) __stub_setjmp(env)
#define _setjmp(env) __stub_setjmp(env)
#define sigsetjmp(env, savemask) __stub_sigsetjmp(env, savemask)

/* longjmp aborts - should never be called in simple utilities */
static inline _Noreturn void longjmp(jmp_buf env, int val) {
    (void)env;
    (void)val;
    abort();
}

static inline _Noreturn void _longjmp(jmp_buf env, int val) {
    (void)env;
    (void)val;
    abort();
}

static inline _Noreturn void siglongjmp(sigjmp_buf env, int val) {
    (void)env;
    (void)val;
    abort();
}

#endif /* _WASI_SETJMP_STUB_H */
