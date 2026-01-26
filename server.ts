const server = Bun.serve({
    port: 3000,
    fetch(req) {
        const url = new URL(req.url)
        let path = url.pathname
        if (path === "/") path = "/playground/index.html"
        if (path.endsWith("/")) path += "index.html"

        const file = Bun.file("." + path)
        return new Response(file, {
            headers: {
                "Cross-Origin-Opener-Policy": "same-origin",
                "Cross-Origin-Embedder-Policy": "require-corp",
            },
        })
    },
})

console.log(`Listening on http://localhost:${server.port}`)
console.log(`Open http://localhost:${server.port}/playground/index.html`)
