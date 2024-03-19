$WshShell = New-Object -comObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut("$PsScriptRoot\..\Open Genshin.lnk")
$Shortcut.TargetPath = "$PsScriptRoot\..\Main-Win64\unlockfps_nc.exe"
$Shortcut.Save()