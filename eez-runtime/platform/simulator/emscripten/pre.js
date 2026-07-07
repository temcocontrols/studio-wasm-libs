module["exports"] = function (postWorkerToRendererMessage) {
    var Module = {};

    Module.postWorkerToRendererMessage = postWorkerToRendererMessage;

    Module.onRuntimeInitialized = function () {
        postWorkerToRendererMessage({ init: {} });
    }

    Module.print = function (args) {};    // silenced — noisy animationFrameLoop etc.
    Module.printErr = function (args) {};   // silenced

    // Fixed absolute path for browser WASM loading.
    // Emscripten's standalone locateFile() delegates to Module["locateFile"].
    // If Module.locateFile is undefined, it falls back to scriptDirectory+path
    // which is empty for dynamically loaded scripts → relative URL → 404.
    Module.locateFile = function (path) {
        console.log('T3-EEZ-Studio: locateFile called for', path);
        if (typeof document !== "undefined") {
            return '/t3-eez-studio/wasm/' + path;
        }
        // Node.js: use the original Emscripten locateFile behavior
        var scripts = document ? document.getElementsByTagName("script") : null;
        if (scripts && scripts.length > 0) {
            var src = scripts[scripts.length - 1].src;
            return src.substring(0, src.lastIndexOf("/") + 1) + path;
        }
        return path;
    };

    runWasmModule(Module);

    return Module;
}

function runWasmModule(Module) {

