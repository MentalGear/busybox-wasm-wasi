# BusyBox WASM/WASI

A port of [BusyBox](https://busybox.net/) to WebAssembly with WASI (WebAssembly System Interface).

## Overview

This project provides BusyBox compiled to WebAssembly, allowing you to run common Unix utilities in any WASI-compatible runtime, including browsers (via wasi-kernel), Wasmtime, Wasmer, and more.

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

# Alternative: Use busybox directly with applet as first argument
wasmtime --dir=. --argv0 busybox busybox.wasm echo "Hello!"
```

**Note:** The `--argv0` flag sets the program name (argv[0]) which BusyBox uses to determine which applet to run. The `--dir=.` grants filesystem access to the current directory.

### Running in Browser (wasi-kernel)

For browser-based execution, use [wasi-kernel](https://github.com/corwin-of-amber/wasi-kernel):

```javascript
import { WASI } from 'wasi-kernel';

const wasi = new WASI({
  args: ['busybox', 'ls', '-la'],
  env: {},
  preopens: { '/': '/' }
});

const wasm = await WebAssembly.compile(await fetch('busybox.wasm').then(r => r.arrayBuffer()));
const instance = await WebAssembly.instantiate(wasm, wasi.getImports());
wasi.start(instance);
```

## Available Commands

BusyBox provides many Unix utilities in a single binary. To see available applets:

```bash
wasmtime busybox.wasm -- --list
```

Common utilities include:
- **File operations:** `ls`, `cat`, `cp`, `mv`, `rm`, `mkdir`, `chmod`, `chown`
- **Text processing:** `grep`, `sed`, `awk`, `sort`, `uniq`, `wc`, `head`, `tail`
- **Shell:** `ash` (Almquist shell)
- **Utilities:** `echo`, `printf`, `date`, `env`, `test`, `expr`

## Filesystem Access

WASI requires explicit filesystem permissions. When running busybox.wasm, you need to grant access to directories:

```bash
# Grant access to current directory
wasmtime --dir=. busybox.wasm -- ls -la

# Grant access to specific directories
wasmtime --dir=/tmp --dir=/home busybox.wasm -- ls /tmp

# With wasmer
wasmer run --dir=. busybox.wasm -- cat file.txt
```

## Examples

### Text Processing Pipeline

```bash
# Using shell features (run ash)
wasmtime --dir=. busybox.wasm ash -c 'echo "hello world" | tr a-z A-Z'

# Process a file
wasmtime --dir=. busybox.wasm -- grep "pattern" file.txt
```

### File Operations

```bash
# List files
wasmtime --dir=. busybox.wasm -- ls -la

# Create directory
wasmtime --dir=. busybox.wasm -- mkdir newdir

# Copy file
wasmtime --dir=. busybox.wasm -- cp source.txt dest.txt
```

### Running the Shell

```bash
# Start interactive ash shell
wasmtime --dir=. busybox.wasm ash
```

## Building from Source

See [WASI_BUILD.md](WASI_BUILD.md) for detailed build instructions.

### Quick Build

```bash
# Install WASI SDK
export WASI_SDK_VERSION=24
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VERSION}/wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
tar xzf wasi-sdk-${WASI_SDK_VERSION}.0-x86_64-linux.tar.gz
export WASI_SDK=$(pwd)/wasi-sdk-${WASI_SDK_VERSION}.0

# Build
make CROSS_COMPILE=${WASI_SDK}/bin/wasm32-wasi- HOSTCC=gcc SKIP_STRIP=y
```

## Limitations

Due to WASI sandbox restrictions:
- **No networking:** Socket operations are not available
- **No process spawning:** `fork()` and `exec()` have limited support
- **No signals:** Signal handling is emulated
- **Filesystem sandbox:** Only pre-opened directories are accessible

## Project Structure

```
busybox-wasm-wasi/
├── busybox.wasm          # Pre-built WASM binary
├── README.md             # This file
├── WASI_BUILD.md         # Build documentation
├── readme_original.md    # Original BusyBox README
├── wasi-kit.json         # WASI build configuration
├── include/wasi/         # WASI compatibility headers
└── ...                   # BusyBox source code
```

## Credits

- **BusyBox:** https://busybox.net/
- **WASI Port:** [Shachar Itzhaky (corwin-of-amber)](https://github.com/corwin-of-amber)
- **wasi-kernel:** https://github.com/corwin-of-amber/wasi-kernel
- **WASI SDK:** https://github.com/WebAssembly/wasi-sdk

## License

BusyBox is licensed under the GNU General Public License version 2. See the original license in the source files.

## References

- [WASI Specification](https://wasi.dev/)
- [Wasmtime Runtime](https://wasmtime.dev/)
- [Wasmer Runtime](https://wasmer.io/)
- [WebAssembly](https://webassembly.org/)
