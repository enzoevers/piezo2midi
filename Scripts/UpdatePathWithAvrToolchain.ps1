param ([bool] $github_ci = $false)

$toolchainPath = resolve-path "$PSScriptRoot\..\avr-toolchain"

echo "Adding AVR toolchain to PATH from $toolchainPath"

if ($github_ci) {
    echo "Detected GitHub CI environment"
    echo "$toolchainPath\avr8-gnu-toolchain\bin\" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append
    echo "$toolchainPath\avr8-gnu-toolchain\avr\bin\" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append
    echo "$toolchainPath\gnuwin32\bin\" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append
    return
}
else {
    $env:PATH += ";$toolchainPath\avr8-gnu-toolchain\bin\"
    $env:PATH += ";$toolchainPath\avr8-gnu-toolchain\avr\bin\"
    $env:PATH += ";$toolchainPath\gnuwin32\bin\"
    return
}