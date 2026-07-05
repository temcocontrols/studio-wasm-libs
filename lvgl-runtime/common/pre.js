// Export the factory function for both CommonJS (Electron/Node) and
// browser environments (globalThis fallback for script-tag loading).
var LVGLWasmRuntime = function (postWorkerToRendererMessage) {
    var Module = {};

    Module.postWorkerToRendererMessage = postWorkerToRendererMessage;

    Module.onRuntimeInitialized = function () {
        if (postWorkerToRendererMessage) {
            postWorkerToRendererMessage({ init: {} });
        }
    }

    Module.print = function (args) {
        console.log("From LVGL-WASM flow runtime:", args);
    };

    Module.printErr = function (args) {
        console.error("From LVGL-WASM flow runtime:", args);
    };

    Module.locateFile = function (path, scriptDirectory) {
        // Prefer the explicit URL set by the host (handles Vite/Quasar path rewriting)
        if (typeof globalThis !== "undefined" && globalThis.__lvglWasmUrl && path.endsWith(".wasm")) {
            return globalThis.__lvglWasmUrl;
        }
        // Fallback: locate next to the loading script
        var scripts = document.getElementsByTagName("script");
        var src = scripts[scripts.length - 1].src;
        return src.substring(0, src.lastIndexOf("/") + 1) + path;
    };

    runWasmModule(Module);

    return Module;
};

// CommonJS export (Electron/Node)
if (typeof module !== "undefined" && module["exports"]) {
    module["exports"] = LVGLWasmRuntime;
}
// Browser global (script-tag loading)
if (typeof globalThis !== "undefined") {
    globalThis.LVGLWasmRuntime = LVGLWasmRuntime;
}

function runWasmModule(Module) {

