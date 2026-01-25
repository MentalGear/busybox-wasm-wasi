# BusyBox WASIX Playground

Interactive browser demos for BusyBox WASIX.

## Contents

### 1. Basic Example (`index.html`)
Simple HTML page running BusyBox commands with @wasmer/sdk.
- Text input for commands
- Example command buttons
- Output display

### 2. Virtual Filesystem Demo (`filesystem.html`)
- Upload files to WASM virtual filesystem
- Run BusyBox commands on uploaded files
- Download results from virtual FS

### 3. Interactive Terminal (`terminal.html`)
Full shell experience using xterm.js:
- Interactive command line
- Command history (up/down arrows)
- Colored output
- Ctrl+C and Ctrl+L support

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

# Then open http://localhost:8080/playground/
```

## File Structure

```
playground/
├── README.md          # This file
├── index.html         # Basic command runner
├── filesystem.html    # Virtual FS demo
└── terminal.html      # Interactive terminal
```

## References

- [Wasmer SDK Docs](https://docs.wasmer.io/sdk/wasmer-js/)
- [XTerm.js](https://xtermjs.org/)
- [webassembly.sh](https://github.com/wasmerio/webassembly.sh)
