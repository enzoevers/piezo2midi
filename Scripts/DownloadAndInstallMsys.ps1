$msysToolPath = "$PSScriptRoot\..\msys2-tool"

if (Test-Path $msysToolPath) {
    Write-Host "MSYS2 tool folder already exists at $(resolve-path $msysToolPath)"
    exit 0
}

mkdir $msysToolPath

$msysToolPath = resolve-path $msysToolPath

curl.exe -L -o $msysToolPath\msys2.exe https://github.com/msys2/msys2-installer/releases/download/2025-12-13/msys2-base-x86_64-20251213.sfx.exe

Invoke-Expression "$msysToolPath\msys2.exe -y -o$msysToolPath"

# rm $msysToolPath\msys2.exe

$env:CHERE_INVOKING = 'yes'
$env:MSYSTEM = 'UCRT64' # https://www.msys2.org/docs/environments/