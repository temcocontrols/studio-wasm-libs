@echo off
echo === BUILD ALL ===
call "%~dp0build-lvgl-all.bat"
cd /d "%~dp0"
call "%~dp0build-core.bat"
echo === BUILD ALL DONE ===
