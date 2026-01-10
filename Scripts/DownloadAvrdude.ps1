$avrdudeDir = "$PSScriptRoot/../avrdude"
$version = "8.1"

if (Test-Path $avrdudeDir) {
    Write-Host "AVRDUDE folder already exists at $avrdudeDir"
    exit 0
}

mkdir $avrdudeDir
$avrdudeDir = resolve-path $avrdudeDir

curl.exe -L -o $avrdudeDir/avrdude.zip "https://github.com/avrdudes/avrdude/releases/download/v$version/avrdude-v$version-windows-x64.zip"
tar -xf $avrdudeDir/avrdude.zip -C $avrdudeDir
