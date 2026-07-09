@echo off
call C:\QN\temcocontrols\emsdk\emsdk_env.bat
set PATH=C:\Program Files\CMake\bin;C:\QN\temcocontrols\ninja;%PATH%
cd /d C:\QN\temcocontrols\studio-wasm-libs\lvgl-runtime\v8.4.0
if not exist build mkdir build
cd build
echo === Configuring v8.4.0 ===
emcmake cmake ..
echo === Building v8.4.0 ===
emmake ninja -j4
echo === v8.4.0 DONE ===
