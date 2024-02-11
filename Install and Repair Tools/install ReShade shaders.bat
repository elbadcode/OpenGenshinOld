
@echo off
setlocal enabledelayedexpansion
start /wait XCOPY "..\Main-Win64\Shade\ReShade.exe" "C:\tempgenshin\ReShade.exe" /-I /Q /Y

start /wait "" "C:\tempgenshin\ReShade.exe" "C:\tempgenshin\ReShade.exe" --api d3d11 --state modify --elevated

cd ..\Main-Win64\Shade\

start /wait XCOPY "C:\tempgenshin\reshade-shaders\*" reshade-shaders\ /Q /I /E /Y


:Clean
rd C:\tempgenshin /s /q
pause