function getLeftEdgePos(image) {
    const { data, width, height } = imageData;
    const channels = 4;
    const bufSize = width * height * channels;
    const inBufPtr = wasm.exports.malloc(bufSize);
    const inBuf = new Uint8Array(wasm.exports.memory.buffer, inBufPtr, bufSize);
    inBuf.set(data);
    const leftEdgePos = wasm.exports.process(inBufPtr, width, height);
    wasm.exports.free(inBufPtr);
}

function filter(imageData) {
    const { data, width, height } = imageData;
    const channels = 4;
    const bufSize = width * height * channels;

    const inBufPtr = wasm.exports.malloc(bufSize);
    const inBuf = new Uint8Array(wasm.exports.memory.buffer, inBufPtr, bufSize);
    inBuf.set(data);

    const outBufPtr = wasm.exports.malloc(bufSize);
    const outBuf = new Uint8Array(wasm.exports.memory.buffer, outBufPtr, bufSize);
    wasm.exports.applyEdgeKernelToImage(inBufPtr, outBufPtr, width, height);
    data.set(outBuf);

    wasm.exports.free(inBufPtr);
    wasm.exports.free(outBufPtr);

    return data;
}


function render(context2d, video, image) {
    context2d.drawImage(video, 0, 0, image.width, image.height);
    const imageData = context2d.getImageData(0, 0, image.width, image.height);
    imageData.data.set(filter(imageData));
    context2d.putImageData(imageData, 0, 0);
    requestAnimationFrame(() => render(context2d, video, image));
}

async function fetchWasmInstance() {
    const { instance } = await WebAssembly.instantiateStreaming(fetch('filter.wasm'), {
        env: {
            emscripten_notify_memory_growth: function() {}
        }
    });
    return instance;
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
    const context2d = image.getContext('2d');
    requestAnimationFrame(() => render(context2d, video, image));
}

let wasm;

// driver
main().catch(console.error);

