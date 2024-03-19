echo off
SETLOCAL EnableDelayedExpansion 
cd /d "%~dp0"

IF "%~1" == "" GOTO :QueryUser
IF NOT "%~1" == "" set a=%1%
Call :Link3dmigotoDir %a% & pause


 

:QueryUser
set /p linkExisting="Do you have an existing 3dmigoto installation you want to link? (y/n): "
if /i "%linkExisting%" == "y" goto :Select3dmigotoDir
if /i "%linkExisting%" == "n" GOTO :Make3dmigotoDir


:Select3dmigotoDir
set "psCommand="(new-object -COM 'Shell.Application')^
.BrowseForFolder(0,'Please choose a folder.',0,0).self.path""
for /f "usebackq delims=" %%I in (`powershell %psCommand%`) do set "folder=%%I"
echo You chose %folder%
Call :Link3dmigotoDir %folder%
pause






:Link3dmigotoDir
CD "%~dp0\Main-Win64\3DM\"
mklink /D  ShaderFixes %1\ShaderFixes
mklink /D   ShaderCache %1\ShaderCache
mklink /D  Mods %1\Mods
XCOPY %1\d3dx.ini  d3dx.ini /Q /-I /C
XCOPY %1\d3dx_user.ini  d3dx_user.ini /Q /-I /C
ECHO 3dmigoto links generated 
EXIT /B 0




endlocal
pause

