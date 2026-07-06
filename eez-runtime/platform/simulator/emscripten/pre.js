module["exports"] = function (postWorkerToRendererMessage) {
    var Module = {};

    Module.postWorkerToRendererMessage = postWorkerToRendererMessage;

    Module.onRuntimeInitialized = function () {
        postWorkerToRendererMessage({ init: {} });
    }

    Module.print = function (args) {
        console.log("From EEZ-WASM flow runtime:", args);
    };

    Module.printErr = function (args) {
        console.error("From EEZ-WASM flow runtime:", args);
    };

    // Intercept Emscripten's Module.locateFile assignment.
    // The generated code overwrites Module.locateFile with its own default,
    // which resolves .wasm paths relative to document.currentScript (may be null
    // for dynamically loaded scripts, causing .quasar/eez_runtime.wasm 404).
    // Using a setter captures the assignment and replaces it with our absolute path.
    var _locateFile;
    Object.defineProperty(Module, 'locateFile', {
        get: function () { return _locateFile; },
        set: function (fn) {
            _locateFile = function (path) {
                if (typeof document !== "undefined") {
                    return '/t3-eez-studio/wasm/' + path;
                }
                return fn(path);  // Node.js fallback
            };
        },
        configurable: true,
        enumerable: true
    });

    runWasmModule(Module);

    return Module;
}

function runWasmModule(Module) {

