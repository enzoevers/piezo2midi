param(
    [string]$TargetDir = $PSScriptRoot,
    [switch]$CheckOnly
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-PythonExecutable {
    $repoRoot = Split-Path -Path $PSScriptRoot -Parent
    $venvPython = Join-Path $repoRoot ".venv\Scripts\python.exe"

    if (Test-Path $venvPython) {
        Write-Host "Using Python executable from virtual environment: $venvPython"
        return $venvPython
    }

    $pythonCommand = Get-Command python -ErrorAction SilentlyContinue
    if ($null -ne $pythonCommand) {
        Write-Host "Using Python executable from system PATH: $($pythonCommand.Source)"
        return $pythonCommand.Source
    }

    throw "Python executable not found. Create/activate a virtual environment or install Python."
}

if (-not (Test-Path $TargetDir)) {
    throw "Target directory does not exist: $TargetDir"
}

$pythonExe = Get-PythonExecutable

$arguments = @("-m", "black", "-q", "--config", "$TargetDir\pyproject.toml")
if ($CheckOnly) {
    $arguments += "--check"
}

$arguments += "$TargetDir"

Write-Host "Formatting Python files under $TargetDir"
& $pythonExe @arguments

$returnCode = $LASTEXITCODE
if ($returnCode -eq 0) {
    Write-Host "Python files are properly formatted."
} elseif ($returnCode -eq 1) {
    Write-Host "Some Python files need formatting. Run the script without -CheckOnly to format them."
} else {
    Write-Host "An error occurred while checking/formatting Python files."
}

exit $LASTEXITCODE