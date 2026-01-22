/* wasi/control.h stub
 * This is a stub for the wasi-kernel control API which provides
 * fork-like semantics for WebAssembly. Since we don't have the
 * full wasi-kernel, we provide minimal stubs.
 */
#ifndef _WASI_CONTROL_H
#define _WASI_CONTROL_H

/* Block extension (Clang blocks) - stub if not available */
#ifndef __block
#define __block
#endif

/* Fork-like control flow API stubs */
typedef void (^fork_block_t)(int);

/* These are stubs - actual implementation would come from wasi-kernel */

#endif /* _WASI_CONTROL_H */
