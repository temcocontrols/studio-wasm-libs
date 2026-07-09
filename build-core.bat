@echo off
call C:\QN\temcocontrols\emsdk\emsdk_env.bat
set PATH=C:\Program Files\CMake\bin;C:\QN\temcocontrols\ninja;%PATH%

cd /d C:\QN\temcocontrols\studio-wasm-libs\eez-runtime
if not exist build mkdir build
cd build
echo === Configuring eez_runtime ===
emcmake cmake ..
echo === Building eez_runtime ===
emmake ninja -j4
echo === eez_runtime DONE ===

cd /d C:\QN\temcocontrols\studio-wasm-libs\eez-gui-lite-runtime
if not exist build mkdir build
cd build
echo === Configuring eez_gui_lite_runtime ===
emcmake cmake ..
echo === Building eez_gui_lite_runtime ===
emmake ninja -j4
echo === eez_gui_lite_runtime DONE ===

cd /d C:\QN\temcocontrols\studio-wasm-libs\lz4
if not exist build mkdir build
cd build
echo === Configuring lz4 ===
emcmake cmake ..
echo === Building lz4 ===
emmake ninja -j4
echo === lz4 DONE ===

echo === CORE DONE ===
