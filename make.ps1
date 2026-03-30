# make.ps1 — build system helper for dsa-cpp
# Usage:
#   .\make.ps1 build      — configure + build everything
#   .\make.ps1 test       — run all test_* executables
#   .\make.ps1 examples   — run all *_demo executables
#   .\make.ps1 clean      — wipe the build folder

param(
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateSet("build", "test", "examples", "clean")]
    [string]$Command
)

$Root      = $PSScriptRoot
$Build     = "$Root\build"
$Compiler  = "C:/msys64/ucrt64/bin/g++.exe"

# ── Helpers ───────────────────────────────────────────────────────────────────
function Write-Header($text) {
    $line = "=" * 60
    Write-Host ""
    Write-Host $line -ForegroundColor Cyan
    Write-Host "  $text" -ForegroundColor Cyan
    Write-Host $line -ForegroundColor Cyan
}

function Invoke-Build {
    Write-Header "BUILD"

    # Configure only if cache is missing
    if (-not (Test-Path "$Build\build.ninja")) {
        Write-Host "-> Configuring with CMake..." -ForegroundColor Yellow
        cmake -S $Root -B $Build -G Ninja `
              "-DCMAKE_CXX_COMPILER=$Compiler"
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[ERROR] CMake configure failed." -ForegroundColor Red
            exit 1
        }
    }

    Write-Host "-> Building..." -ForegroundColor Yellow
    cmake --build $Build
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[ERROR] Build failed." -ForegroundColor Red
        exit 1
    }

    Write-Host ""
    Write-Host "Build OK." -ForegroundColor Green
}

function Invoke-Tests {
    Write-Header "TESTS"

    $exes = Get-ChildItem -Path $Build -Recurse -Filter "test_*.exe"

    if ($exes.Count -eq 0) {
        Write-Host "No test executables found. Run: .\make.ps1 build" -ForegroundColor Yellow
        exit 0
    }

    $passed = 0
    $failed = 0

    foreach ($exe in $exes) {
        Write-Host ""
        Write-Host "-> $($exe.Name)" -ForegroundColor Yellow
        & $exe.FullName
        if ($LASTEXITCODE -eq 0) {
            Write-Host "   PASS" -ForegroundColor Green
            $passed++
        } else {
            Write-Host "   FAIL (exit code $LASTEXITCODE)" -ForegroundColor Red
            $failed++
        }
    }

    Write-Host ""
    Write-Host "Results: $passed passed, $failed failed." -ForegroundColor Cyan
    if ($failed -gt 0) { exit 1 }
}

function Invoke-Examples {
    Write-Header "EXAMPLES"

    $exes = Get-ChildItem -Path $Build -Recurse -Filter "*_demo.exe"

    if ($exes.Count -eq 0) {
        Write-Host "No demo executables found. Run: .\make.ps1 build" -ForegroundColor Yellow
        exit 0
    }

    foreach ($exe in $exes) {
        Write-Host ""
        Write-Host "-> $($exe.Name)" -ForegroundColor Yellow
        Write-Host ("-" * 50) -ForegroundColor DarkGray
        & $exe.FullName
    }

    Write-Host ""
    Write-Host "All demos complete." -ForegroundColor Green
}

function Invoke-Clean {
    Write-Header "CLEAN"
    if (Test-Path $Build) {
        Remove-Item -Recurse -Force $Build
        Write-Host "Deleted: $Build" -ForegroundColor Green
    } else {
        Write-Host "Nothing to clean." -ForegroundColor Yellow
    }
}

# ── Dispatch ──────────────────────────────────────────────────────────────────
switch ($Command) {
    "build"    { Invoke-Build    }
    "test"     { Invoke-Tests    }
    "examples" { Invoke-Examples }
    "clean"    { Invoke-Clean    }
}