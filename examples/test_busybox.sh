#!/bin/bash
# Test script for BusyBox WASM/WASI
# Requires wasmtime to be installed

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUSYBOX_WASM="${SCRIPT_DIR}/../busybox.wasm"

# Check for wasmtime
if ! command -v wasmtime &> /dev/null; then
    echo "Error: wasmtime not found. Please install it:"
    echo "  curl https://wasmtime.dev/install.sh -sSf | bash"
    exit 1
fi

# Helper function to run busybox applet
run_applet() {
    local applet="$1"
    shift
    wasmtime --dir=. --dir=/tmp --argv0 "$applet" "$BUSYBOX_WASM" "$@"
}

echo "BusyBox WASM Test Suite"
echo "WASM binary: $BUSYBOX_WASM"
echo "Runtime: wasmtime $(wasmtime --version 2>/dev/null | head -1)"
echo "=========================================="
echo

# Test 1: Basic echo
echo "Test 1: Basic echo"
run_applet echo "Hello from BusyBox WASM!"
echo

# Test 2: List current directory
echo "Test 2: List current directory"
run_applet ls -la . 2>&1 | head -10
echo

# Test 3: Cat a file
echo "Test 3: Read a file with cat"
run_applet cat "${SCRIPT_DIR}/../README.md" 2>&1 | head -5
echo

# Test 4: Printf formatting
echo "Test 4: Printf formatting"
run_applet printf "Number: %d, String: %s\n" 42 "test"
echo

# Test 5: Environment variables
echo "Test 5: Show environment (env)"
run_applet env 2>&1 | head -5 || echo "(env may show limited results)"
echo

# Test 6: Date
echo "Test 6: Date command"
run_applet date 2>&1 || echo "(date may have limited functionality)"
echo

# Test 7: Test command
echo "Test 7: Test command (conditionals)"
run_applet test 1 -eq 1 && echo "Test passed: 1 equals 1"
echo

# Test 8: Basename
echo "Test 8: Basename"
run_applet basename /path/to/file.txt
echo

# Test 9: Dirname
echo "Test 9: Dirname"
run_applet dirname /path/to/file.txt
echo

# Test 10: pwd
echo "Test 10: Working directory (pwd)"
run_applet pwd 2>&1 || echo "(pwd may not work in WASI sandbox)"
echo

echo "=========================================="
echo "Tests completed!"
echo
echo "Note: Some commands may have limited functionality due to WASI sandbox restrictions."
echo "The shell (ash) is disabled in this build to avoid setjmp/longjmp issues."
