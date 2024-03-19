@echo off
setlocal enabledelayedexpansion

Mkdir "Open Genshin"
CD "Open Genshin"				

powershell -Command "Expand-Archive -Path  '%MYFILES%\OpenGenshin.zip' -DestinationPath '%~dps0\Open Genshin\' -Force"

start /wait XCOPY ".\Main-Win64\Shade\ReShade.exe" "C:\tempgenshin\ReShade.exe" /-I /Q /Y

start /wait "" "C:\tempgenshin\ReShade.exe" "C:\tempgenshin\ReShade.exe" --api d3d11 --state modify --elevated

start /wait XCOPY "C:\tempgenshin\reshade-shaders\*" "%cd%\Main-Win64\Shade\reshade-shaders\" /Q /I /E /Y

:QueryUser
rem ShowSelf
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
CD "%~dps0\Open Genshin\Main-Win64\3DM\"
mklink /D  Main-Win64\3DM\ShaderFixes %1\ShaderFixes
mklink /D   Main-Win64\3DM\ShaderCache %1\ShaderCache
mklink /D  Main-Win64\3DM\Mods %1\Mods
XCOPY %1\d3dx.ini  Main-Win64\3DM\d3dx.ini /Q /-I /C
XCOPY %1\d3dx_user.ini  Main-Win64\3DM\d3dx_user.ini /Q /-I /C
ECHO 3dmigoto links generated 
GOTO Deps

:Make3dmigotoDir
CD "%~dps0\				Open Genshin\Main-Win64\3DM\"
powershell -Command "Invoke-WebRequest 'https://github.com/SilentNightSound/GI-Model-Importer/releases/download/v7.0/3dmigoto-GIMI-for-playing-mods.zip' -OutFile 'C:\tempgenshin\GIMI.zip'"
powershell -Command "Expand-Archive -LiteralPath  'C:\tempgenshin\GIMI.zip' -DestinationPath 'C:\tempgenshin\GIMI' -Force"

XCOPY C:\tempgenshin\gimi\3dmigoto\Mods Mods /Q /I /E /Y

XCOPY C:\tempgenshin\gimi\3dmigoto\ShaderFixes ShaderFixes /Q /I /E /Y
XCOPY C:\tempgenshin\gimi\3dmigoto\d3dx.ini d3dx.ini /Q /-I /C
echo 3dmigoto folders generate! Add mods to Main-Win64\3DM\Mods to use
GOTO Deps

:Deps
powershell -Command "Invoke-WebRequest 'https://dot.net/v1/dotnet-install.ps1' -OutFile 'C:\tempgenshin\dotnet-install.ps1'"
powershell -noexit -executionpolicy bypass  "& 'C:\tempgenshin\dotnet-install.ps1' -InstallDir '~/.dotnet' -Version '8.0.1' -Runtime 'dotnet'"
goto clean

@echo Install complete. 
set /p tutorial="Would you like to view usage instructions and links to more shaders I recommend? (y/n)"
if /i "%tutorial% == "y" start "https://github.com/elbadcode/OpenGenshin/blob/main/PostInstall"
start "" "Open Genshin.exe" --elevated

:Clean
rd C:\tempgenshin /s /q
CLS
