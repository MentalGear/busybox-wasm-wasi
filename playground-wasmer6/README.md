# BusyBox WASIX Playground - Wasmer 6 Edition

Browser-based interactive demos for BusyBox WASIX using **Wasmer SDK 0.6.x**.

## Wasmer 6 Compatibility

This playground is specifically designed for compatibility with **Wasmer Runtime 6.x** and uses the `@wasmer/sdk@0.6.0` package. Since the Wasmer SDK has not been updated to v7, this version ensures compatibility with the current stable SDK.

### SDK Version Details

| Component | Version | Notes |
|-----------|---------|-------|
| WASM Build | `busybox-wasix-wasmer6.wasm` | Versioned for Wasmer 6 |
| @wasmer/sdk | 0.6.0 | Official Wasmer JavaScript SDK |
| Wasmer Runtime | 6.x | CLI runtime compatibility |
| WASIX | Supported | Full WASIX features |

## Demos

### 1. Basic Command Runner (`index.html`)

Simple interface to run BusyBox commands with example buttons.

**Features:**
- Single command execution
- Environment variable support
- Example command quick-buttons

### 2. Virtual Filesystem (`filesystem.html`)

Upload files to a virtual filesystem, run commands on them, and download results.

**Features:**
- Add/remove files to virtual FS
- Run file processing commands
- Download output files

### 3. Interactive Terminal (`terminal.html`)

Full xterm.js-based terminal experience.

**Features:**
- Command history (arrow keys)
- Ctrl+C and Ctrl+L support
- ANSI color output

## Running Locally

These pages require Cross-Origin Isolation headers. Use a server that sets:

```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

### Quick Start Options

**Using Python:**
```bash
cd playground-wasmer6
python3 -c "
from http.server import HTTPServer, SimpleHTTPRequestHandler
class Handler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()
HTTPServer(('localhost', 8080), Handler).serve_forever()
"
```

**Using Node.js:**
```bash
npx serve -l 8080 --cors
# Or with proper headers:
npx http-server -p 8080 --cors -c-1
```

Then open: `http://localhost:8080/`

## API Differences (Wasmer 6 vs 7)

The `@wasmer/sdk@0.6.0` API is similar to newer versions:

```javascript
import { init, runWasix } from '@wasmer/sdk';

// Initialize SDK
await init();

// Compile WASM module
const module = await WebAssembly.compile(wasmBytes);

// Run WASIX program
const instance = await runWasix(module, {
  program: 'busybox',
  args: ['echo', 'Hello'],
  env: { 'HOME': '/' },
  mount: { '/file.txt': data }
});

// Get results
const result = await instance.wait();
console.log(result.stdout);  // Output text
console.log(result.stderr);  // Error text
console.log(result.code);    // Exit code
```

## For Wasmer 7 Users

If you're using Wasmer 7.x runtime and need newer SDK features, see the main `/playground/` folder which uses `@aspect-sh/wasmer-js@latest`.

## License

Same as parent project - GNU General Public License version 2.
