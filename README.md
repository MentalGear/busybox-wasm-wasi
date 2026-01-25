# BusyBox WASIX

A port of [BusyBox](https://busybox.net/) to WebAssembly with WASIX support, including a fully functional `ash` shell.

## Overview

This project provides BusyBox compiled to WebAssembly using [WASIX](https://wasix.org/), an extended version of WASI with better POSIX compatibility. Run common Unix utilities in any WASIX-compatible runtime (Wasmer CLI or browser via wasmer-js).

**Pre-built binary:** `busybox-wasix.wasm` (~608KB)

## Quick Start

### Running with Wasmer CLI

```bash
# Install wasmer (https://wasmer.io/)
curl https://get.wasmer.io -sSfL | sh

# Run BusyBox commands
wasmer run --volume .:/ busybox-wasix.wasm -- echo "Hello from WASIX!"
wasmer run --volume .:/ busybox-wasix.wasm -- ls -la
wasmer run --volume .:/ busybox-wasix.wasm -- cat README.md

# Run the ash shell (interactive)
wasmer run --volume .:/ busybox-wasix.wasm -- ash

# Run shell commands
wasmer run busybox-wasix.wasm -- sh -c 'echo "Hello from shell!"'
wasmer run busybox-wasix.wasm -- sh -c 'for i in 1 2 3; do echo $i; done'
```

**Notes:**
- `--volume .:/` mounts the current directory to root in the WASM filesystem
- Use `--` to separate wasmer options from busybox arguments
- Requires Wasmer 7.0+ for full WASIX compatibility

### Running in Browser (@wasmer/sdk)

```html
<!DOCTYPE html>
<html>
<head>
  <title>BusyBox WASIX</title>
</head>
<body>
  <pre id="output"></pre>
  <script type="module">
    import { init, runWasix } from 'https://unpkg.com/@wasmer/sdk@latest/dist/index.mjs';

    async function run() {
      await init();

      const wasm = await fetch('busybox-wasix.wasm').then(r => r.arrayBuffer());
      const module = await WebAssembly.compile(wasm);

      const instance = await runWasix(module, {
        program: 'busybox',
        args: ['ls', '-la'],
        env: {}
      });

      const output = await instance.wait();
      document.getElementById('output').textContent = output.stdout;
    }

    run();
  </script>
</body>
</html>
```

**Note:** Pages using `@wasmer/sdk` require Cross-Origin Isolation headers:
- `Cross-Origin-Opener-Policy: same-origin`
- `Cross-Origin-Embedder-Policy: require-corp`

For more details, see the [Wasmer SDK documentation](https://docs.wasmer.io/sdk/wasmer-js/).

## Available Commands

### Enabled Applets

The following 60+ applets are included:

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

### Full Applet Availability

| Applet | Status | Notes |
|--------|--------|-------|
| **Coreutils** | | |
| basename | ✅ | |
| cat | ✅ | |
| chgrp | ❌ | Requires user/group DB |
| chmod | ✅ | |
| chown | ❌ | Stub only |
| chroot | ❌ | Not supported |
| cksum | ❌ | |
| comm | ❌ | |
| cp | ✅ | |
| cut | ✅ | |
| date | ✅ | |
| dd | ❌ | |
| df | ❌ | No filesystem stats |
| dirname | ✅ | |
| dos2unix | ❌ | |
| du | ❌ | |
| echo | ✅ | |
| env | ✅ | |
| expr | ✅ | |
| false | ✅ | |
| head | ✅ | |
| id | ❌ | Requires user DB |
| ln | ✅ | |
| ls | ✅ | |
| md5sum | ❌ | |
| mkdir | ✅ | |
| mktemp | ✅ | |
| mv | ✅ | |
| od | ❌ | |
| printenv | ✅ | |
| printf | ✅ | |
| pwd | ✅ | |
| readlink | ✅ | |
| realpath | ✅ | |
| rm | ✅ | |
| rmdir | ✅ | |
| seq | ✅ | |
| sleep | ✅ | |
| sort | ✅ | |
| stat | ✅ | |
| tac | ✅ | |
| tail | ✅ | |
| tee | ✅ | |
| test/[/[[ | ✅ | |
| touch | ✅ | |
| tr | ✅ | |
| true | ✅ | |
| tty | ✅ | |
| uname | ✅ | |
| uniq | ✅ | |
| wc | ✅ | |
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
| sed | ✅ | |
| vi | ❌ | Requires terminal |
| **Shell** | | |
| ash/sh | ✅ | Full functionality |
| **Console** | | |
| clear | ✅ | |
| reset | ✅ | |
| **Archival** | | |
| tar/gzip/bzip2 | ❌ | |
| unzip | ❌ | |
| **Networking** | | |
| wget/nc/ping | ❌ | No sockets yet |

**Legend:** ✅ = Working | ❌ = Not available

## Shell Features

The `ash` shell supports:

```bash
# Variables
wasmer run busybox-wasix.wasm -- sh -c 'NAME=World; echo "Hello $NAME"'

# Arithmetic
wasmer run busybox-wasix.wasm -- sh -c 'echo $((2 + 2))'

# Conditionals
wasmer run busybox-wasix.wasm -- sh -c 'if test 5 -gt 3; then echo yes; fi'

# Loops
wasmer run busybox-wasix.wasm -- sh -c 'for i in 1 2 3; do echo $i; done'

# Functions
wasmer run busybox-wasix.wasm -- sh -c 'greet() { echo "Hello $1"; }; greet World'
```

### Working Directory (cd)

The `cd` command is a shell builtin (not a standalone applet). It works within shell scripts:

```bash
# Change to directory and verify with pwd
wasmer run --volume .:/ busybox-wasix.wasm -- sh -c 'cd /app && pwd'

# Navigate with relative paths
wasmer run --volume .:/ busybox-wasix.wasm -- sh -c 'cd /app/examples && cd .. && pwd'

# Chain multiple cd operations
wasmer run --volume .:/ busybox-wasix.wasm -- sh -c 'cd /app && cd examples && cd .. && cd include && pwd'
```

### Known Limitation

Pipelines within shell scripts don't work because fork+exec cannot find busybox applets (they're compiled into the single binary). Use host-side pipes instead:

```bash
# This works (host-side pipe):
echo "hello" | wasmer run busybox-wasix.wasm -- tr a-z A-Z

# This doesn't work (shell-internal pipe):
wasmer run busybox-wasix.wasm -- sh -c 'echo hello | tr a-z A-Z'
```

## Filesystem Access

Mount host directories using `--volume`:

```bash
# Mount current directory to /app
wasmer run --volume .:/app busybox-wasix.wasm -- ls /app

# Mount multiple directories
wasmer run --volume .:/app --volume /tmp:/tmp busybox-wasix.wasm -- ls /

# Mount as root
wasmer run --volume .:/ busybox-wasix.wasm -- ls -la
```

## Building from Source

### Prerequisites

- [wasixcc](https://github.com/aspect-build/wasixcc) - WASIX C compiler
- GCC (for host tools)
- Make

### Build Steps

```bash
# Install wasixcc
cargo install wasixcc --features=tracing-subscriber
wasixcc --download-all

# Configure BusyBox
make defconfig
# Enable ash shell and desired applets in menuconfig
make menuconfig

# Build
make CROSS_COMPILE=/path/to/wasm32-wasix- HOSTCC=gcc SKIP_STRIP=y

# The output is busybox (rename to busybox-wasix.wasm)
mv busybox busybox-wasix.wasm
```

## Project Structure

```
busybox-wasix/
├── busybox-wasix.wasm    # Pre-built WASIX binary
├── README.md             # This file
├── examples/
│   └── test_wasix.sh     # Test suite (27 passing tests)
├── include/wasi/
│   └── wasix_compat.h    # WASIX compatibility declarations
└── libbb/
    └── wasi_stubs.c      # POSIX function stubs
```

## Running Tests

```bash
./examples/test_wasix.sh
```

Output:
```
BusyBox WASIX Test Suite
==========================================
=== Core Applet Tests ===
Test: echo... PASS
Test: ls... PASS
...
=== Shell (ash) Tests ===
Test: shell echo... PASS
Test: shell variables... PASS
...
=== Working Directory (cd) Tests ===
Test: cd absolute... PASS
...
==========================================
Results: 27 passed, 0 failed
All tests passed!
```

## Credits

- **BusyBox:** https://busybox.net/
- **WASIX:** https://wasix.org/
- **Wasmer:** https://wasmer.io/

## License

BusyBox is licensed under the GNU General Public License version 2.

## References

- [WASIX Specification](https://wasix.org/)
- [Wasmer Runtime](https://wasmer.io/)
- [Wasmer SDK (@wasmer/sdk)](https://docs.wasmer.io/sdk/wasmer-js/)
- [WebAssembly](https://webassembly.org/)
