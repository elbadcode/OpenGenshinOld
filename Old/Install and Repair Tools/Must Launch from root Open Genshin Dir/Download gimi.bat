powershell -Command "Invoke-WebRequest 'https://github.com/SilentNightSound/GI-Model-Importer/releases/download/v7.0/3dmigoto-GIMI-for-playing-mods.zip' -OutFile 'C:\tempgenshin\GIMI.zip'"
powershell -Command "Expand-Archive -LiteralPath 'C:\tempgenshin\GIMI.zip' -DestinationPath C:\tempgenshin\GIMI"

XCOPY C:\tempgenshin\gimi\3dmigoto\Mods Mods /Q /I /E /Y

XCOPY C:\tempgenshin\gimi\3dmigoto\ShaderFixes ShaderFixes /Q /I /E /Y
XCOPY C:\tempgenshin\gimi\3dmigoto\ Mods /Q /I /E /Y