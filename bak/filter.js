function filter(imageData) {
    const { data, width, height } = imageData;
    const channels = 4;
    const bufSize = width*height*channels;
    const inBufPtr = wasm.exports._malloc(bufSize);
    const outBufPtr = wasm.exports._malloc(bufSize);
    const inBuf = new Uint8Array(wasm.instance.exports.memory.buffer, inBufPtr, bufSize);
    const outBuf = new Uint8Array(wasm.instance.exports.memory.buffer, outBufPtr, bufSize);

    bufferIn.set(data);

    wasmModule.apply_edge_kernel_to_image(inBufPtr, outBufPtr, width, height);

    data.set(outBuf);

    wasm.exports._free(inBufPtr);
    wasm.exports._free(outBufPtr);

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
    const { instance } = await WebAssembly.instantiateStreaming(fetch('filter.wasm'));
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

