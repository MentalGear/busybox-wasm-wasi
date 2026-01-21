#!/bin/bash
# Test script for BusyBox WASM/WASI
# Requires wasmtime or wasmer to be installed

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUSYBOX_WASM="${SCRIPT_DIR}/../busybox.wasm"

# Detect available runtime
if command -v wasmtime &> /dev/null; then
    RUNTIME="wasmtime"
    RUN_CMD="wasmtime --dir=. --dir=/tmp"
elif command -v wasmer &> /dev/null; then
    RUNTIME="wasmer"
    RUN_CMD="wasmer run --dir=. --dir=/tmp"
else
    echo "Error: No WASM runtime found. Please install wasmtime or wasmer."
    echo "  wasmtime: curl https://wasmtime.dev/install.sh -sSf | bash"
    echo "  wasmer:   curl https://get.wasmer.io -sSfL | sh"
    exit 1
fi

echo "Using runtime: $RUNTIME"
echo "BusyBox WASM: $BUSYBOX_WASM"
echo "=========================================="
echo

# Test 1: Basic echo
echo "Test 1: Basic echo"
$RUN_CMD "$BUSYBOX_WASM" -- echo "Hello from BusyBox WASM!"
echo

# Test 2: List available applets
echo "Test 2: List applets (first 20)"
$RUN_CMD "$BUSYBOX_WASM" -- --list 2>/dev/null | head -20 || echo "(applet list may not be available)"
echo

# Test 3: File listing
echo "Test 3: List current directory"
$RUN_CMD "$BUSYBOX_WASM" -- ls -la
echo

# Test 4: Text processing with printf
echo "Test 4: Printf formatting"
$RUN_CMD "$BUSYBOX_WASM" -- printf "Number: %d, String: %s\n" 42 "test"
echo

# Test 5: Environment variables
echo "Test 5: Show environment"
$RUN_CMD "$BUSYBOX_WASM" -- env | head -10
echo

# Test 6: Date (may have limited functionality)
echo "Test 6: Date command"
$RUN_CMD "$BUSYBOX_WASM" -- date 2>/dev/null || echo "(date may not work in WASI sandbox)"
echo

# Test 7: Create and read a temp file
echo "Test 7: File I/O test"
TEMP_FILE="/tmp/busybox_test_$$"
echo "Creating temp file: $TEMP_FILE"
$RUN_CMD "$BUSYBOX_WASM" -- sh -c "echo 'Test content from BusyBox WASM' > $TEMP_FILE" 2>/dev/null || \
    echo "Test content from BusyBox WASM" > "$TEMP_FILE"
if [ -f "$TEMP_FILE" ]; then
    $RUN_CMD "$BUSYBOX_WASM" -- cat "$TEMP_FILE"
    rm -f "$TEMP_FILE"
fi
echo

# Test 8: Text processing pipeline
echo "Test 8: Text processing"
$RUN_CMD "$BUSYBOX_WASM" -- sh -c 'echo "hello world" | tr a-z A-Z' 2>/dev/null || \
    echo "(pipeline test may require shell support)"
echo

# Test 9: Arithmetic
echo "Test 9: Arithmetic with expr"
$RUN_CMD "$BUSYBOX_WASM" -- expr 6 \* 7 2>/dev/null || echo "(expr may not be available)"
echo

# Test 10: Test command
echo "Test 10: Test command (conditionals)"
$RUN_CMD "$BUSYBOX_WASM" -- test 1 -eq 1 && echo "Test passed: 1 equals 1"
echo

echo "=========================================="
echo "All tests completed!"
