# BusyBox WASM/WASI

A port of [BusyBox](https://busybox.net/) to WebAssembly with WASI (WebAssembly System Interface).

## Overview

This project provides BusyBox compiled to WebAssembly, allowing you to run common Unix utilities in any WASI-compatible runtime, including Wasmtime, and browsers (via wasi-kernel).

**Pre-built binary:** `busybox.wasm` (~785KB)

## Quick Start

### Running with Wasmtime

```bash
# Install wasmtime (https://wasmtime.dev/)
curl https://wasmtime.dev/install.sh -sSf | bash

# Run BusyBox commands using --argv0 to specify the applet name
wasmtime --dir=. --argv0 echo busybox.wasm "Hello from WASM!"
wasmtime --dir=. --argv0 ls busybox.wasm -la
wasmtime --dir=. --argv0 cat busybox.wasm file.txt
wasmtime --dir=. --argv0 date busybox.wasm

# Alternative: Use busybox directly with applet as first argument
wasmtime --dir=. --argv0 busybox busybox.wasm echo "Hello!"
```

**Important Notes:**
- The `--argv0` flag sets the program name (argv[0]) which BusyBox uses to determine which applet to run
- The `--dir=.` grants filesystem access to the current directory
- Without `--dir`, BusyBox cannot access any files

### Running in Browser (wasi-kernel)

For browser-based execution, use [wasi-kernel](https://github.com/corwin-of-amber/wasi-kernel):

```javascript
import { WASI } from 'wasi-kernel';

const wasi = new WASI({
  args: ['echo', 'Hello from browser!'],  // First arg is applet name
  env: {},
  preopens: { '/': '/' }
});

const wasm = await WebAssembly.compile(await fetch('busybox.wasm').then(r => r.arrayBuffer()));
const instance = await WebAssembly.instantiate(wasm, wasi.getImports());
wasi.start(instance);
```

## Available Commands

Common utilities that work in this build:
- **File operations:** `ls`, `cat`, `cp`, `mv`, `rm`, `mkdir`, `touch`, `chmod`
- **Text processing:** `grep`, `sed`, `sort`, `uniq`, `wc`, `head`, `tail`, `tr`, `cut`
- **Path utilities:** `basename`, `dirname`, `pwd`, `realpath`, `readlink`
- **Output:** `echo`, `printf`, `yes`
- **Comparison:** `test`, `expr`, `cmp`
- **Other:** `date`, `env`, `sleep`, `seq`, `tee`, `xargs`

**Note:** The shell (`ash`) is disabled in this build due to setjmp/longjmp requirements that need the WebAssembly exceptions proposal.

## Filesystem Access

WASI requires explicit filesystem permissions:

```bash
# Grant access to current directory
wasmtime --dir=. --argv0 ls busybox.wasm -la

# Grant access to multiple directories
wasmtime --dir=. --dir=/tmp --argv0 cp busybox.wasm file.txt /tmp/

# Map host directory to guest path
wasmtime --dir=/home/user::/ --argv0 ls busybox.wasm /
```

## Examples

```bash
# List files
wasmtime --dir=. --argv0 ls busybox.wasm -la

# Read a file
wasmtime --dir=. --argv0 cat busybox.wasm README.md

# Show date
wasmtime --dir=. --argv0 date busybox.wasm

# Text processing
wasmtime --dir=. --argv0 grep busybox.wasm "pattern" file.txt

# Path manipulation
wasmtime --dir=. --argv0 basename busybox.wasm /path/to/file.txt
wasmtime --dir=. --argv0 dirname busybox.wasm /path/to/file.txt
```

## Building from Source

### Prerequisites

- WASI SDK 24+ (https://github.com/WebAssembly/wasi-sdk)
- GCC (for host tools)
- Make

### Build Steps

```bash
# Download and extract WASI SDK
export WASI_SDK_VERSION=24
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VERSION}/wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
tar xzf wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
export WASI_SDK=$(pwd)/wasi-sdk-${WASI_SDK_VERSION}.0

# Create toolchain symlinks
mkdir -p /tmp/wasi-toolchain
ln -sf ${WASI_SDK}/bin/clang /tmp/wasi-toolchain/wasm32-wasi-gcc
ln -sf ${WASI_SDK}/bin/llvm-ar /tmp/wasi-toolchain/wasm32-wasi-ar

# Build BusyBox
make CROSS_COMPILE=/tmp/wasi-toolchain/wasm32-wasi- HOSTCC=gcc SKIP_STRIP=y
```

See [WASI_BUILD.md](WASI_BUILD.md) for detailed build documentation.

## Implementation Details

### WASI Compatibility Layer

This build includes custom stubs for POSIX functions not available in WASI:

- **`libbb/wasi_stubs.c`** - Stub implementations for ~50 POSIX functions:
  - Process management (`fork`, `exec`, `wait`, `getpid`, `getuid`, etc.)
  - Signal handling (`signal`, `sigaction`, `kill`, `raise`, etc.)
  - Terminal I/O (`tcgetattr`, `tcsetattr`, termios functions)
  - Resource limits (`getrlimit`, `setrlimit`, `getrusage`)
  - Temporary files (`mkstemp`, `mkdtemp`, etc.)

- **`include/wasi/setjmp_stub.h`** - Stub setjmp/longjmp that bypasses the WebAssembly exceptions proposal requirement

- **`scripts/trylink`** - Modified to force-link libbb with `--whole-archive` ensuring the main function is included

### Build Configuration

Key `.config` settings for WASI:
```
CONFIG_EXTRA_CFLAGS="-include include/wasi/setjmp_stub.h -D_WASI_EMULATED_SIGNAL -D_WASI_EMULATED_MMAN -Iinclude/wasi"
CONFIG_EXTRA_LDFLAGS="-Wl,-allow-undefined"
CONFIG_EXTRA_LDLIBS="-lwasi-emulated-signal -lwasi-emulated-mman -lwasi-emulated-process-clocks -lwasi-emulated-getpid"
```

## Limitations

Due to WASI sandbox restrictions:
- **No shell:** `ash` is disabled (requires setjmp with exceptions proposal)
- **No networking:** Socket operations are not available
- **No process spawning:** `fork()` and `exec()` return errors
- **No real signals:** Signal handlers are stored but never invoked
- **Filesystem sandbox:** Only pre-opened directories are accessible
- **Stub functions:** Many syscalls return success but don't actually do anything (e.g., `chown`, `chmod` in some cases)

## Project Structure

```
busybox-wasm-wasi/
├── busybox.wasm              # Pre-built WASM binary
├── README.md                 # This file
├── WASI_BUILD.md             # Build documentation
├── readme_original.md        # Original BusyBox README
├── examples/
│   ├── test_busybox.sh       # Test script for wasmtime
│   ├── run_busybox.mjs       # Node.js WASI example
│   └── browser.html          # Browser demo
├── include/wasi/
│   ├── setjmp_stub.h         # Setjmp stubs (no exceptions)
│   ├── signal_extra.h        # Signal definitions
│   ├── termios.h             # Terminal I/O definitions
│   ├── unistd_extra.h        # Additional POSIX declarations
│   └── sys/                  # System header stubs
├── libbb/
│   └── wasi_stubs.c          # POSIX function stubs
└── scripts/
    └── trylink               # Modified linker script
```

## Future Work

### WASIX Support

[WASIX](https://wasix.org/) is an extended WASI specification that adds support for:
- Full POSIX threads
- Process forking and spawning
- Berkeley sockets (networking)
- Proper signal handling
- setjmp/longjmp without exceptions

Adding WASIX support would enable:
- The `ash` shell with full functionality
- Networking applets (`wget`, `nc`, `ping`, etc.)
- Process control (`ps`, `kill`, etc.)
- More complete POSIX compatibility

To build with WASIX, you would need to:
1. Use a WASIX-compatible toolchain (e.g., wasix-libc)
2. Enable the shell and networking in `.config`
3. Link against WASIX libraries instead of WASI emulation libraries

See: https://github.com/aspect-build/aspect-js-wasi for WASIX runtime support.

### Other Potential Improvements

- Enable more applets as WASI/WASIX support improves
- Add WebAssembly Component Model support
- Optimize binary size with `wasm-opt`
- Add automated testing in CI

## Credits

- **BusyBox:** https://busybox.net/
- **WASI Port:** [Shachar Itzhaky (corwin-of-amber)](https://github.com/corwin-of-amber)
- **wasi-kernel:** https://github.com/corwin-of-amber/wasi-kernel
- **WASI SDK:** https://github.com/WebAssembly/wasi-sdk

## License

BusyBox is licensed under the GNU General Public License version 2. See the original license in the source files.

## References

- [WASI Specification](https://wasi.dev/)
- [WASIX Specification](https://wasix.org/)
- [Wasmtime Runtime](https://wasmtime.dev/)
- [Wasmer Runtime](https://wasmer.io/)
- [WebAssembly](https://webassembly.org/)
