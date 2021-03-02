# WASM Image Filter

Minimalist WASM module that applies a "Laplacian" kernel to an image in order to find edges, by highlighting regions of rapid intensity change (i.e. where the Laplacian changes sign).

Javascript code drives the program by feeding images to the WASM module from the user's video.

The WASM is compiled from C code, using the Emscripten compiler, `emcc`. Emscripten's mission is to make your C code 'just work' as WebAssembly. To that end, they provide a good amount of Javascript glue code, many configuration options for the compiler. `emcc` can be used to produce not only the .wasm file, but also .js and .html files containing this glue code.

You can also just tell `emcc` to only output a .wasm file. This file is still particular to Emscripten - another WASM compiler won't produce exactly the same WASM. But the process of fetching the '.wasm' file and initializing a WebAssembly module in Javascript, getting an instance of the module and calling exported functions defined in C code (or whatever language), is not particular to Emscripten. 

Since I've avoided relying on Emscripten's (totally dope) API, it's likely that the way I've done it here will be reproducible with minor tweaks using other WASM compilers. Ideally, all that will need to change is the command for compiling - not the C code doing the native computations, nor the Javascript driver code doing the WASM loading and image feeding.

# To compile
`./run_emcc`

# To serve
e.g.
`python -m http.server`
