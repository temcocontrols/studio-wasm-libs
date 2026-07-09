@echo off
echo === LVGL ALL ===
cd /d "%~dp0"
call build-lvgl-84.bat
call build-lvgl-92.bat
call build-lvgl-9x.bat
echo === LVGL ALL DONE ===
