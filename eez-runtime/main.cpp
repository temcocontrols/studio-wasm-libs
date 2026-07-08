#include <stdio.h>
#include <emscripten.h>

#include <eez/gui/gui.h>
#include <eez/gui/display.h>
#include <eez/gui/thread.h>
#include <eez/flow/flow.h>
#include <eez/flow/hooks.h>
#include <eez/flow/debugger.h>
#include <eez/flow/components.h>
#include <eez/flow/flow_defs_v3.h>
#include <eez/flow/date.h>

#include "./gui/keypad.h"

static int g_started = false;

uint32_t DISPLAY_WIDTH;
uint32_t DISPLAY_HEIGHT;

extern void eez_system_tick();

// clang-format off
void mountFileSystem() {
    EM_ASM(
        FS.mkdir("/min_eez_sample");
        FS.mount(IDBFS, {}, "/min_eez_sample");

        //Module.print("start file sync..");

        //flag to check when data are synchronized
        Module.syncdone = 0;

        FS.syncfs(true, function(err) {
            assert(!err);
            //Module.print("end file sync..");
            Module.syncdone = 1;
        });
    , 0);
}
// clang-format on

void startToDebuggerMessage() {
    EM_ASM({
        startToDebuggerMessage($0);
    }, eez::flow::g_wasmModuleId);
}

static char g_debuggerBuffer[1024 * 1024];
static uint32_t g_debuggerBufferIndex = 0;

void writeDebuggerBuffer(const char *buffer, uint32_t length) {
    if (g_debuggerBufferIndex + length > sizeof(g_debuggerBuffer)) {
        EM_ASM({
            writeDebuggerBuffer($0, new Uint8Array(Module.HEAPU8.buffer, $1, $2));
        }, eez::flow::g_wasmModuleId, g_debuggerBuffer, g_debuggerBufferIndex);
        g_debuggerBufferIndex = 0;
    }
    memcpy(g_debuggerBuffer + g_debuggerBufferIndex, buffer, length);
    g_debuggerBufferIndex += length;
}

void finishToDebuggerMessage() {
    if (g_debuggerBufferIndex > 0) {
        EM_ASM({
            writeDebuggerBuffer($0, new Uint8Array(Module.HEAPU8.buffer, $1, $2));
        }, eez::flow::g_wasmModuleId, g_debuggerBuffer, g_debuggerBufferIndex);
        g_debuggerBufferIndex = 0;
    }

    EM_ASM({
        finishToDebuggerMessage($0);
    }, eez::flow::g_wasmModuleId);
}

EM_PORT_API(void) stopScript() {
    eez::flow::stop();
}

namespace eez {
    namespace flow {
        void executeScpiComponent(FlowState *flowState, unsigned componentIndex);
    }
}

EM_PORT_API(void) init(uint32_t wasmModuleId, uint32_t debuggerMessageSubsciptionFilter, uint8_t *assets, uint32_t assetsSize, uint32_t displayWidth, uint32_t displayHeight, bool darkTheme, uint32_t timeZone, bool screensLifetimeSupport) {
    eez::flow::g_wasmModuleId = wasmModuleId;

    eez::flow::date::g_timeZone = timeZone;

    eez::initAssetsMemory();
    eez::loadMainAssets(assets, assetsSize);
    DISPLAY_WIDTH = displayWidth;
    DISPLAY_HEIGHT = displayHeight;
    eez::initOtherMemory();
    eez::initAllocHeap(eez::ALLOC_BUFFER, eez::ALLOC_BUFFER_SIZE);

    eez::flow::startToDebuggerMessageHook = startToDebuggerMessage;
    eez::flow::writeDebuggerBufferHook = writeDebuggerBuffer;
    eez::flow::finishToDebuggerMessageHook = finishToDebuggerMessage;

    eez::flow::stopScriptHook = stopScript;
    eez::flow::showKeyboardHook = eez::gui::showKeyboard;
    eez::flow::showKeypadHook = eez::gui::showKeypad;
    eez::flow::registerComponent(eez::flow::defs_v3::COMPONENT_TYPE_SCPI_ACTION, eez::flow::executeScpiComponent);

    eez::flow::setDebuggerMessageSubsciptionFilter(debuggerMessageSubsciptionFilter);
    eez::flow::onDebuggerClientConnected();

    // Start the flow engine — creates flow states for all pages.
    // Without this, g_isStopped remains true and getPageFlowState()
    // always returns nullptr, so no flow states are ever created.
    eez::flow::start(eez::g_mainAssets);

    eez::gui::startThread();
    eez::gui::display::turnOn();
}

// Trigger page flow state creation from JS.
// pageId is 1-based (matches the C++ convention: page index + 1).
EM_PORT_API(void) pageChanged(int previousPageId, int activePageId) {
    eez::flow::onPageChanged(previousPageId, activePageId);
}

// Directly create a page flow state and return its index, or -1 on failure.
EM_PORT_API(int) initPageFlowState(int pageIndex) {
#if EEZ_OPTION_GUI
    WidgetCursor widgetCursor;
    auto flowState = eez::flow::getPageFlowState(eez::g_mainAssets, pageIndex, widgetCursor);
#else
    auto flowState = eez::flow::getPageFlowState(eez::g_mainAssets, pageIndex);
#endif
    return flowState ? flowState->flowStateIndex : -1;
}

EM_PORT_API(bool) mainLoop() {
    if (!g_started) {
        mountFileSystem();
        g_started = true;
    } else {
        if (emscripten_run_script_int("Module.syncdone") == 1) {
            if (eez::flow::isFlowStopped()) {
                return false;
            }

            eez_system_tick();

            if (eez::flow::isFlowStopped()) {
                return false;
            }

            // clang-format off
            EM_ASM(
                if (Module.syncdone) {
                    //Module.print("Start File sync..");
                    Module.syncdone = 0;

                    FS.syncfs(false, function(err) {
                        assert(!err);
                        //Module.print("End File sync..");
                        Module.syncdone = 1;
                    });
                }
            , 0);
            // clang-format on
        }
    }

    return true;
}

EM_PORT_API(void) onMessageFromDebugger(char *messageData, uint32_t messageDataSize) {
    eez::flow::processDebuggerInput(messageData, messageDataSize);
}
