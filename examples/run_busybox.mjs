/**
 * Node.js example for running BusyBox WASM with WASI
 *
 * Usage:
 *   node run_busybox.mjs ls -la
 *   node run_busybox.mjs echo "Hello World"
 *   node run_busybox.mjs cat /etc/passwd
 *
 * Requires Node.js 20+ with WASI support
 */

import { readFile } from 'node:fs/promises';
import { WASI } from 'node:wasi';
import { argv, env } from 'node:process';
import { fileURLToPath } from 'node:url';
import { dirname, join } from 'node:path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Get command line arguments (skip node and script name)
const busyboxArgs = argv.slice(2);

if (busyboxArgs.length === 0) {
    console.log('BusyBox WASM/WASI - Node.js Runner');
    console.log('');
    console.log('Usage: node run_busybox.mjs <command> [args...]');
    console.log('');
    console.log('Examples:');
    console.log('  node run_busybox.mjs ls -la');
    console.log('  node run_busybox.mjs echo "Hello World"');
    console.log('  node run_busybox.mjs cat file.txt');
    console.log('  node run_busybox.mjs --list');
    process.exit(0);
}

// Initialize WASI with filesystem access
const wasi = new WASI({
    version: 'preview1',
    args: ['busybox', ...busyboxArgs],
    env: {
        PATH: '/bin:/usr/bin',
        HOME: '/home',
        USER: 'wasi',
        ...env
    },
    preopens: {
        '/': '/',
        '.': process.cwd()
    }
});

try {
    // Load the WASM binary
    const wasmPath = join(__dirname, '..', 'busybox.wasm');
    const wasmBuffer = await readFile(wasmPath);

    // Compile and instantiate
    const wasm = await WebAssembly.compile(wasmBuffer);
    const instance = await WebAssembly.instantiate(wasm, wasi.getImportObject());

    // Run the WASI application
    wasi.start(instance);
} catch (error) {
    if (error.code === 'ENOENT') {
        console.error('Error: busybox.wasm not found');
        console.error('Make sure you are running from the examples directory');
    } else {
        console.error('Error:', error.message);
    }
    process.exit(1);
}
