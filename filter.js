function filter(imageData) {
    const { data, width, height } = imageData;
    const channels = 4;
    const bufSize = width*height*channels;
    const inBufPtr = wasm.exports.malloc(bufSize);
    const outBufPtr = wasm.exports.malloc(bufSize);
    const inBuf = new Uint8Array(wasm.exports.memory.buffer, inBufPtr, bufSize);
    const outBuf = new Uint8Array(wasm.exports.memory.buffer, outBufPtr, bufSize);

    inBuf.set(data);

    wasm.exports.apply_edge_kernel_to_image(inBufPtr, outBufPtr, width, height);

    data.set(outBuf);

    wasm.exports.free(inBufPtr);
    wasm.exports.free(outBufPtr);

    return data;
}

function renderSource(source, dest) {
    const context = dest.getContext('2d');
    context.drawImage(source, 0, 0, dest.width, dest.height);

    const imageData = context.getImageData(0, 0, dest.width, dest.height);
    imageData.data.set(filter(imageData));
    context.putImageData(imageData, 0, 0);

    requestAnimationFrame(_ => renderSource(source, dest));
}

async function fetchWasmInstance() {
    const response = await fetch('filter.wasm');

    // not the most efficient way to load/compile WASM from server
    //const wasmFile = await response.arrayBuffer();
    //const compiledModule = await WebAssembly.compile(wasmFile);
    //const {module, instance} = await WebAssembly.instantiate(compiledModule);
    //

    // most efficient
    const { instance } = await WebAssembly.instantiateStreaming(fetch('filter.wasm'), {
        env: {
            emscripten_notify_memory_growth: function() {},
        }
    });
    return instance;
    //

}

async function main() {
    const video = document.createElement('video');
    const stream = await navigator.mediaDevices.getUserMedia({ audio: false, video: true });

    wasm = await fetchWasmInstance();

    video.setAttribute(`width`, 640);
    video.setAttribute(`height`, 480);
    video.srcObject = stream;
    video.play();

    const image = document.querySelector('#image');
    requestAnimationFrame(_ => renderSource(video, image));
}

let wasm;

// driver
main().catch(console.error);

