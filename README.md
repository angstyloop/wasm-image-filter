# WASM Image Filter

WASM module that applies kernels and transformations and things and things to 
images fed in from, e.g., a webcam.

# Things

Laplacian, Sobel, grayscale, RGB, CIEXYZ, Luv. 

Detect the width of vertical cinematic bars.

Javascript code drives the program by feeding images to the WASM module from the user's video.

# emcc

The WASM is compiled from C code, using the Emscripten compiler, `emcc`. Emscripten's mission is to make your C code 'just work' as WebAssembly. To that end, they provide a good amount of Javascript glue code, many configuration options for the compiler. `emcc` can be used to produce not only the .wasm file, but also .js and .html files containing this glue code.

You can also just tell `emcc` to only output a .wasm file. This file is still particular to Emscripten - another WASM compiler won't produce exactly the same WASM. But the process of fetching the '.wasm' file and initializing a WebAssembly module in Javascript, getting an instance of the module and calling exported functions defined in C code (or whatever language), is not particular to Emscripten. 

# To compile
`./run_emcc`

# To serve
e.g.
`python -m http.server`

# To test the C code
Most of the C files have unit tests written in the files. During normal
compilation, the unit tests are excluded from any TU using preprocessor macros.
The unit tests can be compiled and run on a file-by-file basis by using the
command listed at the top of each file in a single commented line.

# VIM to yank and run the line
Start somewhere on the line.

Type the following sequences. Don't press ENTER unless instructed.


Yank it with the following sequence of commands.

ESC
0
v
$
h
y

Paste the commnad to the Vim command line, and execute it.

:
!
CTRL+r
"
ENTER


# References
[1] https://github.com/nothings/stb
[2] https://pjreddie.com/courses/computer-vision/
[3] https://emscripten.org/
