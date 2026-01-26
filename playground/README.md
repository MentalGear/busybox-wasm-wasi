# BusyBox WASIX Playground

Interactive browser demos for BusyBox WASIX.

## Current State

No panics in sdk 9.0 but direct exit code: 1
*   **SDK Version**: Downgraded to `@wasmer/sdk@0.9.0` (via `esm.sh`) to resolve "Not able to serialize module" panics observed in `0.10.0+`.
*   **WASM Loading**: Using `WebAssembly.compile()` to pass the compiled `Module` object to `runWasix`.
*   **Server**: Using `bun server.ts` (Port 3000) to reliably serve required security headers.
*   **Headers**:
    *   `Cross-Origin-Opener-Policy: same-origin`
    *   `Cross-Origin-Embedder-Policy: require-corp`
*   **Status**: `window.crossOriginIsolated` is confirmed `true`.


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
# run from root, not /playground, as that way:
# serve.json sets the right CORS headers
# .wasm file in the root can also be served
npx serve
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
