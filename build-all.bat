@echo off
echo === BUILD ALL ===
cd /d "%~dp0"
call build-lvgl-all.bat
call build-core.bat
echo === BUILD ALL DONE ===
