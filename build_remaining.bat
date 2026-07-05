@echo off
call C:\QN\temcocontrols\emsdk\emsdk_env.bat
set PATH=C:\Program Files\CMake\bin;C:\QN\temcocontrols\ninja;%PATH%

for %%v in (v9.3.0 v9.4.0 v9.5.0) do (
    cd C:\QN\temcocontrols\studio-wasm-libs\lvgl-runtime\%%v
    if not exist build mkdir build
    cd build
    echo === Configuring %%v ===
    emcmake cmake ..
    echo === Building %%v ===
    emmake ninja -j4
    echo === %%v DONE ===
)

echo === ALL VERSIONS BUILT ===
