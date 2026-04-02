$msysToolPath = resolve-path "$PSScriptRoot\..\msys2-tool"

Invoke-Expression "$msysToolPath\msys64\ucrt64\bin\qemu-system-avr.exe --version"