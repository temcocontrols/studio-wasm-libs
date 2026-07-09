@echo off
echo === LVGL ALL ===
call "%~dp0build-lvgl-84.bat"
cd /d "%~dp0"
call "%~dp0build-lvgl-92.bat"
cd /d "%~dp0"
call "%~dp0build-lvgl-9x.bat"
echo === LVGL ALL DONE ===
