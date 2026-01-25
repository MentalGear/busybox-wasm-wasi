# BusyBox WASM/WASI/WASIX

A port of [BusyBox](https://busybox.net/) to WebAssembly with WASI and WASIX support.

## Overview

This project provides BusyBox compiled to WebAssembly, allowing you to run common Unix utilities in any WASI-compatible runtime, including Wasmtime, Wasmer, and browsers (via wasi-kernel).

**Pre-built binaries:**
- `busybox.wasm` (~785KB) - WASI build (wasmtime compatible, no shell)
- `busybox-wasix.wasm` (~608KB) - WASIX build (wasmer compatible, **with shell**)

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

### Running with Wasmer (WASIX)

The WASIX build includes the `ash` shell and supports more POSIX features:

```bash
# Install wasmer (https://wasmer.io/)
curl https://get.wasmer.io -sSfL | sh

# Run BusyBox commands
wasmer run --volume=.:/ busybox-wasix.wasm -- echo "Hello from WASIX!"
wasmer run --volume=.:/ busybox-wasix.wasm -- ls -la
wasmer run --volume=.:/ busybox-wasix.wasm -- date

# Run the ash shell (interactive)
wasmer run --volume=.:/ busybox-wasix.wasm -- ash

# Run a shell script
echo 'echo "Hello from shell!"' | wasmer run --volume=.:/ busybox-wasix.wasm -- ash
```

**WASIX Benefits:**
- Includes the `ash` shell (not available in WASI build)
- Better POSIX compatibility via WASIX extensions
- Smaller binary size (~608KB vs ~785KB)

**Note:** WASIX requires Wasmer 7.0+ for full compatibility. Earlier versions may have issues with newer WASIX syscalls.

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

### Enabled Applets (Current Build)

The following 60+ applets are enabled in the current WASIX build:

| Category | Applets |
|----------|---------|
| **Shell** | `ash`, `sh` |
| **File Ops** | `cat`, `cp`, `ln`, `ls`, `mkdir`, `mktemp`, `mv`, `rm`, `rmdir`, `touch` |
| **Text Processing** | `cut`, `grep`, `egrep`, `fgrep`, `head`, `sed`, `sort`, `tac`, `tail`, `tr`, `uniq`, `wc` |
| **Path Utils** | `basename`, `dirname`, `pwd`, `readlink`, `realpath` |
| **Output** | `echo`, `printf`, `yes`, `tee` |
| **Comparison** | `test`, `[`, `[[`, `cmp`, `expr` |
| **File Info** | `stat`, `chmod`, `find`, `which` |
| **Environment** | `env`, `printenv`, `date`, `uname`, `whoami`, `tty` |
| **Other** | `clear`, `reset`, `sleep`, `seq`, `xargs`, `true`, `false`, `readahead` |

**Note:** The shell (`ash`) is only available in the WASIX build (`busybox-wasix.wasm`). The WASI build disables the shell due to setjmp/longjmp requirements.

### All BusyBox Applets (Availability)

Below is a comprehensive table of BusyBox applets and their availability:

| Applet | Enabled | Notes |
|--------|---------|-------|
| **Coreutils** | | |
| basename | ✅ | |
| cat | ✅ | |
| chgrp | ❌ | Requires user/group DB |
| chmod | ✅ | |
| chown | ❌ | Stub only (no effect) |
| chroot | ❌ | Not supported in WASI |
| cksum | ❌ | |
| comm | ❌ | |
| cp | ✅ | |
| cut | ✅ | |
| date | ✅ | |
| dd | ❌ | |
| df | ❌ | No filesystem stats in WASI |
| dirname | ✅ | |
| dos2unix/unix2dos | ❌ | |
| du | ❌ | |
| echo | ✅ | |
| env | ✅ | |
| expand/unexpand | ❌ | |
| expr | ✅ | |
| factor | ❌ | |
| false | ✅ | |
| fold | ❌ | |
| head | ✅ | |
| hostid | ❌ | |
| id | ❌ | Requires user DB |
| install | ❌ | |
| link/unlink | ❌ | |
| ln | ✅ | |
| logname | ❌ | |
| ls | ✅ | |
| md5sum/sha*sum | ❌ | |
| mkdir | ✅ | |
| mkfifo | ❌ | Not supported in WASI |
| mknod | ❌ | Stub only |
| mktemp | ✅ | |
| mv | ✅ | |
| nice | ❌ | No process priority in WASI |
| nl | ❌ | |
| nohup | ❌ | |
| nproc | ❌ | |
| od | ❌ | |
| paste | ❌ | |
| printenv | ✅ | |
| printf | ✅ | |
| pwd | ✅ | |
| readlink | ✅ | |
| realpath | ✅ | |
| rm | ✅ | |
| rmdir | ✅ | |
| seq | ✅ | |
| shred | ❌ | |
| shuf | ❌ | |
| sleep | ✅ | |
| sort | ✅ | |
| split | ❌ | |
| stat | ✅ | |
| stty | ❌ | Limited terminal support |
| sum | ❌ | |
| sync | ❌ | |
| tac | ✅ | |
| tail | ✅ | |
| tee | ✅ | |
| test/[/[[ | ✅ | |
| timeout | ❌ | No signals in WASI |
| touch | ✅ | |
| tr | ✅ | |
| true | ✅ | |
| truncate | ❌ | |
| tsort | ❌ | |
| tty | ✅ | |
| uname | ✅ | |
| uniq | ✅ | |
| usleep | ❌ | |
| uudecode/uuencode | ❌ | |
| wc | ✅ | |
| who | ❌ | |
| whoami | ✅ | Returns stub value |
| yes | ✅ | |
| **Findutils** | | |
| find | ✅ | |
| grep/egrep/fgrep | ✅ | |
| xargs | ✅ | |
| which | ✅ | |
| **Editors** | | |
| awk | ❌ | Large, complex |
| cmp | ✅ | |
| diff | ❌ | |
| ed | ❌ | |
| patch | ❌ | |
| sed | ✅ | |
| vi | ❌ | Requires terminal |
| **Shell** | | |
| ash/sh | ✅ | WASIX only |
| hush | ❌ | |
| **Console Tools** | | |
| clear | ✅ | |
| reset | ✅ | |
| **Archival** | | |
| ar | ❌ | |
| bunzip2/bzip2 | ❌ | |
| cpio | ❌ | |
| gunzip/gzip | ❌ | |
| lzop | ❌ | |
| tar | ❌ | |
| unzip | ❌ | |
| xz/unxz | ❌ | |
| **Networking** | | |
| ftpget/ftpput | ❌ | No sockets |
| hostname | ❌ | |
| ifconfig | ❌ | No network interfaces |
| nc/netcat | ❌ | No sockets |
| ping | ❌ | No raw sockets |
| telnet | ❌ | No sockets |
| tftp | ❌ | No sockets |
| wget | ❌ | No sockets |
| **Process/System** | | |
| free | ❌ | No memory info |
| kill | ❌ | No signals |
| killall | ❌ | No process management |
| pgrep/pkill | ❌ | No /proc |
| pidof | ❌ | No /proc |
| ps | ❌ | No /proc |
| renice | ❌ | No priority |
| top | ❌ | No /proc |
| uptime | ❌ | No system info |
| watch | ❌ | |

**Legend:** ✅ = Enabled and working | ❌ = Not enabled or not supported

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
├── busybox.wasm              # Pre-built WASI binary (wasmtime)
├── busybox-wasix.wasm        # Pre-built WASIX binary (wasmer, with shell)
├── README.md                 # This file
├── WASI_BUILD.md             # Build documentation
├── readme_original.md        # Original BusyBox README
├── examples/
│   ├── test_busybox.sh       # Test script for wasmtime
│   ├── run_busybox.mjs       # Node.js WASI example
│   └── browser.html          # Browser demo
├── include/wasi/
│   ├── setjmp_stub.h         # Setjmp stubs (WASI only)
│   ├── wasix_compat.h        # WASIX compatibility declarations
│   ├── signal_extra.h        # Signal definitions (WASI only)
│   ├── termios.h             # Terminal I/O definitions (WASI only)
│   ├── unistd_extra.h        # Additional POSIX declarations (WASI only)
│   └── sys/                  # System header stubs
├── libbb/
│   └── wasi_stubs.c          # POSIX function stubs (conditional for WASI/WASIX)
└── scripts/
    └── trylink               # Modified linker script
```

## WASIX Support

[WASIX](https://wasix.org/) is now supported! The `busybox-wasix.wasm` binary includes:
- The `ash` shell with full functionality
- Improved POSIX compatibility
- setjmp/longjmp support (via WASIX stack switching)

### Building for WASIX

```bash
# Install wasixcc
cargo install wasixcc --features=tracing-subscriber

# Download WASIX toolchain
wasixcc --download-all

# Build (creates a wrapper script for the cross-compiler)
make CROSS_COMPILE=/path/to/wasm32-wasix- HOSTCC=gcc SKIP_STRIP=y
```

See the build configuration files for details on the WASIX-specific modifications.

## Future Work

### Potential Improvements

- Enable networking applets (`wget`, `nc`, `ping`, etc.) - requires WASIX socket support
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
