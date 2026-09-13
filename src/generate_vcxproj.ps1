# ========================================================
#  Generate NFSBundleExplorer.vcxproj for VS 2022 (x86)
# ========================================================

# Set working directory to script location
Set-Location -Path $PSScriptRoot

Write-Host "========================================================" -ForegroundColor Cyan
Write-Host " Generating NFSBundleExplorer.vcxproj via PowerShell" -ForegroundColor Cyan
Write-Host "========================================================`n" -ForegroundColor Cyan

# 1. Path to your 32-bit Qt build
$QtBin = "F:\Qt5.15.14-Windows-x86-VS2019-16.11.37\bin"

if (-not (Test-Path "$QtBin\qmake.exe")) {
    Write-Host "[ERROR] qmake.exe not found at:" -ForegroundColor Red
    Write-Host "$QtBin" -ForegroundColor Yellow
    Write-Host "Please verify the `$QtBin path variable at the top of this script!`n" -ForegroundColor Red
    Read-Host "Press Enter to exit..."
    exit 1
}

# Add Qt to PATH for current PowerShell session
$env:PATH = "$QtBin;$env:PATH"

# 2. Locate Visual Studio 2022 via vswhere.exe
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsPath = $null

if (Test-Path $vswhere) {
    # Find any edition of VS 2022 (version 17.x)
    $vsPath = & $vswhere -latest -version "[17.0,18.0)" -property installationPath
}

# Fallback to standard directories if vswhere is not available
if (-not $vsPath) {
    $standardPaths = @(
        "$env:ProgramFiles\Microsoft Visual Studio\2022\Community",
        "$env:ProgramFiles\Microsoft Visual Studio\2022\Professional",
        "$env:ProgramFiles\Microsoft Visual Studio\2022\Enterprise"
    )
    foreach ($path in $standardPaths) {
        if (Test-Path $path) {
            $vsPath = $path
            break
        }
    }
}

if (-not $vsPath) {
    Write-Host "[ERROR] Visual Studio 2022 was not found!" -ForegroundColor Red
    Write-Host "Ensure that VS 2022 and 'Desktop development with C++' are installed.`n" -ForegroundColor Red
    Read-Host "Press Enter to exit..."
    exit 1
}

$vcvars32 = "$vsPath\VC\Auxiliary\Build\vcvars32.bat"

if (-not (Test-Path $vcvars32)) {
    Write-Host "[ERROR] x86 build environment script not found at:" -ForegroundColor Red
    Write-Host "$vcvars32" -ForegroundColor Yellow
    Read-Host "Press Enter to exit..."
    exit 1
}

Write-Host "[INFO] Visual Studio 2022 found at:" -ForegroundColor Green
Write-Host "$vsPath`n" -ForegroundColor Gray

# 3. Setup x86 environment and execute qmake
Write-Host "[INFO] Initializing x86 environment and running qmake..." -ForegroundColor Green

$cmdArgs = "/c `"`"$vcvars32`" > nul && qmake -spec win32-msvc -tp vc NFSBundleExplorer.pro`""
$process = Start-Process -FilePath "cmd.exe" -ArgumentList $cmdArgs -NoNewWindow -Wait -PassThru

# 4. Check result
if ($process.ExitCode -eq 0 -and (Test-Path "NFSBundleExplorer.vcxproj")) {
    Write-Host "`n========================================================" -ForegroundColor Green
    Write-Host " [SUCCESS] NFSBundleExplorer.vcxproj created successfully!" -ForegroundColor Green
    Write-Host "========================================================`n" -ForegroundColor Green
} else {
    Write-Host "`n[ERROR] qmake failed to generate the project file.`n" -ForegroundColor Red
}

Read-Host "Press Enter to exit..."