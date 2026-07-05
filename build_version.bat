@echo off
call C:\QN\temcocontrols\emsdk\emsdk_env.bat
set PATH=C:\Program Files\CMake\bin;C:\QN\temcocontrols\ninja;%PATH%
cd C:\QN\temcocontrols\studio-wasm-libs\lvgl-runtime\%1
if not exist build mkdir build
cd build
echo === Configuring %1 ===
emcmake cmake ..
echo === Building %1 ===
emmake ninja -j4
echo === %1 DONE ===
