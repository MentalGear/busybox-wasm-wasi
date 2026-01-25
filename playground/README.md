# BusyBox WASIX Playground

Interactive browser demos for BusyBox WASIX.

## Status: Planned

This folder will contain browser-based demos. See [Future Work](../README.md#4-browser-playground) in main README.

## Planned Contents

### 1. Basic Example (`index.html`)
Simple HTML page running BusyBox commands with @wasmer/sdk.

### 2. Virtual Filesystem Demo (`filesystem.html`)
- Upload files to WASM virtual filesystem
- Run BusyBox commands on uploaded files
- Download results

### 3. Interactive Terminal (`terminal.html`)
Full shell experience using:
- [@wasmer/wasm-terminal](https://www.npmjs.com/package/@wasmer/wasm-terminal)
- [xterm.js](https://xtermjs.org/)

Based on [webassembly.sh](https://github.com/wasmerio/webassembly.sh).

## Requirements

Pages require Cross-Origin Isolation headers:
```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

## Local Development

```bash
# Simple server with required headers (Python)
python3 -c "
from http.server import HTTPServer, SimpleHTTPRequestHandler
class Handler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()
HTTPServer(('localhost', 8080), Handler).serve_forever()
"

# Or use a tool like vite/webpack with appropriate headers
```

## References

- [Wasmer SDK Docs](https://docs.wasmer.io/sdk/wasmer-js/)
- [XTerm.js Tutorial](https://docs.wasmer.io/sdk/wasmer-js/tutorials/xterm-js)
- [webassembly.sh Source](https://github.com/wasmerio/webassembly.sh)
