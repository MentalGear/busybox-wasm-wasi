#!/bin/bash
# Test script for BusyBox WASIX
# Requires wasmer 7.0+ with WASIX support

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUSYBOX_WASM="${SCRIPT_DIR}/../busybox-wasix.wasm"
PROJECT_DIR="${SCRIPT_DIR}/.."

# Find wasmer - prefer the one built from source
if [ -x /tmp/wasmer-git/target/release/wasmer ]; then
    WASMER=/tmp/wasmer-git/target/release/wasmer
elif command -v wasmer &> /dev/null; then
    WASMER=wasmer
else
    echo "Error: wasmer not found. Need wasmer 7.0+ for WASIX support."
    echo "Build from source: git clone https://github.com/wasmerio/wasmer.git && cd wasmer && cargo build --release -p wasmer-cli"
    exit 1
fi

# Helper function to run busybox applet
run_applet() {
    "$WASMER" run --volume "${PROJECT_DIR}:/app" -- "$BUSYBOX_WASM" "$@"
}

# Helper for shell commands
run_shell() {
    "$WASMER" run -- "$BUSYBOX_WASM" sh -c "$1"
}

echo "BusyBox WASIX Test Suite"
echo "WASM binary: $BUSYBOX_WASM"
echo "Runtime: $WASMER ($($WASMER --version 2>/dev/null | head -1))"
echo "=========================================="
echo

PASS=0
FAIL=0

run_test() {
    local name="$1"
    shift
    echo -n "Test: $name... "
    if "$@" > /tmp/test_output.txt 2>&1; then
        echo "PASS"
        ((PASS++)) || true
    else
        echo "FAIL"
        cat /tmp/test_output.txt
        ((FAIL++)) || true
    fi
}

check_output() {
    local name="$1"
    local expected="$2"
    shift 2
    echo -n "Test: $name... "
    local result
    result=$("$@" 2>&1)
    if [ "$result" = "$expected" ]; then
        echo "PASS"
        ((PASS++)) || true
    else
        echo "FAIL (expected: '$expected', got: '$result')"
        ((FAIL++)) || true
    fi
}

echo "=== Core Applet Tests ==="

# Test 1: Basic echo
run_test "echo" run_applet echo "Hello WASIX"

# Test 2: List directory
run_test "ls" run_applet ls /app

# Test 3: Cat file
run_test "cat" run_applet cat /app/README.md

# Test 4: Printf formatting
check_output "printf" "Number: 42" run_applet printf "Number: %d" 42

# Test 5: Test command
run_test "test" run_applet test 1 -eq 1

# Test 6: Basename
check_output "basename" "file.txt" run_applet basename /path/to/file.txt

# Test 7: Dirname
check_output "dirname" "/path/to" run_applet dirname /path/to/file.txt

# Test 8: Head
run_test "head" run_applet head -n 1 /app/README.md

# Test 9: Tail
run_test "tail" run_applet tail -n 1 /app/README.md

# Test 10: Wc
run_test "wc" run_applet wc -l /app/README.md

# Test 11: Grep
run_test "grep" run_applet grep WASM /app/README.md

# Test 12: Tr (direct applet call)
check_output "tr (direct)" "HELLO" bash -c 'echo hello | '"$WASMER"' run -- '"$BUSYBOX_WASM"' tr a-z A-Z'

# Test 13: Expr
check_output "expr" "4" run_applet expr 2 + 2

# Test 14: Sort
run_test "sort" bash -c 'echo -e "c\na\nb" | '"$WASMER"' run -- '"$BUSYBOX_WASM"' sort'

echo
echo "=== Shell (ash) Tests ==="

# Test 15: Basic shell
check_output "shell echo" "hello world" run_shell 'echo hello world'

# Test 16: Shell variables
check_output "shell variables" "hello world" run_shell 'X=world; echo hello $X'

# Test 17: Shell arithmetic
check_output "shell arithmetic" "8" run_shell 'echo $((5+3))'

# Test 18: Shell conditionals
check_output "shell if/else" "yes" run_shell 'if test 5 -gt 3; then echo yes; else echo no; fi'

# Test 19: Shell for loop
check_output "shell for loop" "1 2 3 " run_shell 'for i in 1 2 3; do echo -n "$i "; done; echo'

# Test 20: Shell while loop
check_output "shell while loop" "0 1 2 " run_shell 'i=0; while test $i -lt 3; do echo -n "$i "; i=$((i+1)); done; echo'

# Test 21: Shell functions
check_output "shell functions" "hello from func" run_shell 'myfunc() { echo "hello from func"; }; myfunc'

# Test 22: Shell pwd
check_output "shell pwd" "/" run_shell 'pwd'

echo
echo "=== Known Limitations ==="
echo "NOTE: Pipelines within shell scripts don't work in WASIX because"
echo "fork+exec cannot find busybox applets (they're compiled into the binary)."
echo "Use direct applet calls with host-side pipes instead."
echo

echo "=========================================="
echo "Results: $PASS passed, $FAIL failed"
echo

if [ $FAIL -gt 0 ]; then
    exit 1
fi

echo "All tests passed!"
