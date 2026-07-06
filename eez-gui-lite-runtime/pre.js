module["exports"] = function (postWorkerToRendererMessage) {
    var Module = {};

    Module.postWorkerToRendererMessage = postWorkerToRendererMessage;

    Module.onRuntimeInitialized = function () {
        postWorkerToRendererMessage({ init: {} });
    }

    Module.print = function (args) {
        console.log("From eez-gui-lite-runtime:", args);
    };

    Module.printErr = function (args) {
        console.error("From eez-gui-lite-runtime:", args);
    };

    Module.locateFile = function (path, scriptDirectory) {
        if (typeof document !== "undefined") {
            var scripts = document.getElementsByTagName("script");
            var src = scripts[scripts.length - 1].src;
            return src.substring(0, src.lastIndexOf("/") + 1) + path;
        }
        if (scriptDirectory) return scriptDirectory + path;
        return __dirname + "/" + path;
    };

    runWasmModule(Module);

    return Module;
}

function runWasmModule(Module) {

