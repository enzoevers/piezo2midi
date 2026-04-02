$avrToolchainDir = "$PSScriptRoot/../avr-toolchain"

if (Test-Path $avrToolchainDir) {
    Write-Host "AVR toolchain folder already exists at $avrToolchainDir"
    exit 0
}

mkdir $avrToolchainDir

$avrToolchainDir = resolve-path $avrToolchainDir

# Use curl.exe instead of curl to avoid using the Invoke-WebRequest alias in PowerShell

mkdir $avrToolchainDir/avr8-gnu-toolchain
curl.exe -L -o $avrToolchainDir/avr8-gnu-toolchain.zip https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-4.0.0.52-win32.any.x86_64.zip
tar -xf $avrToolchainDir/avr8-gnu-toolchain.zip -C $avrToolchainDir/avr8-gnu-toolchain --strip-components=1

mkdir $avrToolchainDir/gnuwin32
curl.exe -L -o $avrToolchainDir/make-bin.zip https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81-bin.zip/download
tar -xf $avrToolchainDir/make-bin.zip -C $avrToolchainDir/gnuwin32
curl.exe -L -o $avrToolchainDir/make-dep.zip https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81-dep.zip/download
tar -xf $avrToolchainDir/make-dep.zip -C $avrToolchainDir/gnuwin32