@echo off
call C:\QN\temcocontrols\emsdk\emsdk_env.bat
set PATH=C:\Program Files\CMake\bin;C:\QN\temcocontrols\ninja;%PATH%
cd /d C:\QN\temcocontrols\studio-wasm-libs\lvgl-runtime\v9.2.2
if not exist build mkdir build
cd build
echo === Configuring v9.2.2 ===
emcmake cmake ..
echo === Building v9.2.2 ===
emmake ninja -j4
echo === v9.2.2 DONE ===
