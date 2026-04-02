$msysToolPath = resolve-path "$PSScriptRoot\..\msys2-tool"

Invoke-Expression "$msysToolPath\msys64\usr\bin\bash -lc 'pacman --noconfirm -S mingw-w64-ucrt-x86_64-qemu'"