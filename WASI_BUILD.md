# Building BusyBox for WASM/WASI

This document describes how to build BusyBox for WebAssembly with WASI (WebAssembly System Interface).

## Overview

This is a port of BusyBox to WASM/WASI by Shachar Itzhaky (corwin-of-amber). The port includes:
- Network code disabled via `HAVE_NET` macro
- A forkless shell implementation for ash
- WASI-specific compilation settings via `wasi-kit.json`

## Prerequisites

### 1. WASI SDK

Install the WASI SDK from https://github.com/WebAssembly/wasi-sdk

```bash
# Example: Download and extract WASI SDK
export WASI_SDK_VERSION=24
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VERSION}/wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
tar xzf wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
export WASI_SDK=/path/to/wasi-sdk-${WASI_SDK_VERSION}.0
```

### 2. wasi-kit (Optional Build Wrapper)

The `wasi-kit.json` configuration file is designed for use with the `wasi-kit` tool from the `wasi-kernel` project:

```bash
npm install -g wasi-kernel
```

See: https://github.com/corwin-of-amber/wasi-kernel

## Build Configuration

### wasi-kit.json

The `wasi-kit.json` file specifies per-file compilation options:

```json
{
    "shell/ash.o": {
        "args": ["-fblocks", "-Wno-shadow"]
    },
    "busybox_unstripped": {
        "output": "busybox.wasm",
        "args": ["-Wl,-allow-undefined", "-Wl,--export-table"]
    }
}
```

Key flags:
- `-fblocks`: Clang blocks extension (for WASI fork emulation)
- `-Wl,-allow-undefined`: Allow undefined symbols (for WASI runtime)
- `-Wl,--export-table`: Export function table for WebAssembly

### Additional WASI Libraries

For full compatibility, you may need:
- `-lwasi-emulated-mman`: Memory management emulation (mmap, etc.)
- `-lwasi-emulated-signal`: Signal handling emulation
- `-D_WASI_EMULATED_MMAN`: Enable mman emulation defines

## Build Methods

### Method 1: Using wasi-kit (Recommended)

If you have `wasi-kit` installed:

```bash
# Configure BusyBox
make defconfig

# Build with wasi-kit (handles wasi-kit.json automatically)
wasi-kit make
```

### Method 2: Manual Build with WASI SDK

```bash
# Set up environment
export WASI_SDK=/path/to/wasi-sdk
export CC="${WASI_SDK}/bin/clang --sysroot ${WASI_SDK}/share/wasi-sysroot"
export AR="${WASI_SDK}/bin/llvm-ar"
export RANLIB="${WASI_SDK}/bin/llvm-ranlib"

# Configure BusyBox (minimal config recommended)
make defconfig
# Or create a minimal .config for WASI

# Set cross-compilation prefix
make CROSS_COMPILE="${WASI_SDK}/bin/"

# Additional CFLAGS for WASI
export CFLAGS="-D_WASI_EMULATED_MMAN -O3"
export LDFLAGS="-lwasi-emulated-mman -lwasi-emulated-signal -Wl,-allow-undefined"

make
```

### Method 3: Using Makefile Variables

```bash
make \
    CC="${WASI_SDK}/bin/clang --sysroot ${WASI_SDK}/share/wasi-sysroot" \
    AR="${WASI_SDK}/bin/llvm-ar" \
    HOSTCC=gcc \
    EXTRA_CFLAGS="-D_WASI_EMULATED_MMAN" \
    EXTRA_LDFLAGS="-lwasi-emulated-mman -lwasi-emulated-signal -Wl,-allow-undefined"
```

## WASI-Specific Code Changes

### 1. Platform Detection (`include/platform.h`)

```c
#if defined(__wasi__)
# undef HAVE_MNTENT_H
# undef HAVE_SYS_STATFS_H
# undef HAVE_NET
#endif
```

### 2. Network Disabled

The `HAVE_NET` macro disables networking code since WASI lacks socket primitives.

### 3. Shell Fork Strategies (`shell/ash.c`)

The ash shell has multiple fork strategies for WASI:

```c
#ifdef __wasi__
#define __wasik__           // Enable WASI-K features
#define __wasik_setjmp__    // Use setjmp for control flow
//#define __wasik_fork__    // Use WASI control API fork
#define __wasix_vfork__     // Use vfork (WASIX variant)
#endif
```

Options:
- `__wasik__`: Enables WASI control features (requires wasi/control.h)
- `__wasik_fork__`: Uses WASI control API for fork emulation
- `__wasix_vfork__`: Uses vfork() (for WASIX-compatible runtimes)

## Recommended Minimal Configuration

For WASI builds, disable these features in `.config`:
- `CONFIG_FEATURE_INSTALLER=n` (no /proc/self/exe)
- `CONFIG_FEATURE_SUID=n` (no setuid in WASI)
- All networking applets
- Mount-related applets
- Process management applets that require signals

## Output

The build produces:
- `busybox_unstripped` - Unstripped WebAssembly binary
- `busybox.wasm` - Final WebAssembly module (per wasi-kit.json)

## Running the WASM Binary

### With Wasmtime

```bash
wasmtime busybox.wasm ls -la
```

### With wasmer

```bash
wasmer run busybox.wasm -- ls -la
```

### With wasi-kernel (browser)

The binary can run in browsers using the wasi-kernel runtime:
- https://github.com/corwin-of-amber/wasi-kernel

## References

- WASI SDK: https://github.com/WebAssembly/wasi-sdk
- wasi-kernel: https://github.com/corwin-of-amber/wasi-kernel
- WASI specification: https://wasi.dev/
- BusyBox: https://busybox.net/
