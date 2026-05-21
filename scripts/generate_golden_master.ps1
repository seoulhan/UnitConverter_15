# Generates tests/golden_master_expected.txt by capturing UnitConverter stdout.
# Usage:
#   .\scripts\generate_golden_master.ps1 [-ExePath build\unit_converter.exe] [-Force]

param(
    [string]$ExePath = "",
    [switch]$Force
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
if (-not $ExePath) {
    $ExePath = Join-Path $Root "build\unit_converter.exe"
}

$Scenarios = @("meter:2.5", "feet:1.0", "yard:1.0", "meter:0.0")
$ExpectedPath = Join-Path $Root "tests\golden_master_expected.txt"
$PromptPrefix = "Insert value for converting"

if (-not (Test-Path $ExePath)) {
    Write-Error "Executable not found: $ExePath. Build first: cmake -B build -S . ; cmake --build build"
}

function Get-ScenarioStdout {
    param([string]$InputLine)
    $tempOut = [System.IO.Path]::GetTempFileName()
    try {
        $inputLine | & $ExePath 2>$null | Set-Content -Path $tempOut -Encoding utf8
        $lines = Get-Content -Path $tempOut -Encoding utf8 | Where-Object {
            $_ -notlike "$PromptPrefix*"
        }
        if ($null -eq $lines) {
            return ""
        }
        return ($lines -join "`n")
    }
    finally {
        Remove-Item -Path $tempOut -Force -ErrorAction SilentlyContinue
    }
}

$document = New-Object System.Text.StringBuilder
for ($i = 0; $i -lt $Scenarios.Count; $i++) {
    $scenario = $Scenarios[$i]
    [void]$document.AppendLine("[$scenario]")
    $body = Get-ScenarioStdout -InputLine $scenario
    if ($body) {
        [void]$document.AppendLine($body)
    }
    [void]$document.Append("---")
    if ($i -lt ($Scenarios.Count - 1)) {
        [void]$document.AppendLine()
    }
}
[void]$document.AppendLine()

if (Test-Path $ExpectedPath) {
    $existing = Get-Content -Path $ExpectedPath -Raw -Encoding utf8
    $generated = $document.ToString()
    if ($existing -eq $generated) {
        Write-Host "Golden master unchanged: $ExpectedPath"
        exit 0
    }
    if (-not $Force) {
        Write-Error "Baseline exists and differs. Re-run with -Force to overwrite, or fix the CLI first."
    }
}

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
[System.IO.File]::WriteAllText($ExpectedPath, $document.ToString(), $utf8NoBom)
Write-Host "Wrote $ExpectedPath"
Write-Host "Next: git add tests/golden_master_expected.txt"
