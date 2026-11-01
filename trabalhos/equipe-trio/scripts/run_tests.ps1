param(
    [string]$Exe = "",
    [string[]]$TestDirs = @("tests", "..\..\testes")
)

$root = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $root "build"

if (-not $Exe) {
    if (-not (Test-Path $buildDir)) {
        cmake -S $root -B $buildDir
        if ($LASTEXITCODE -ne 0) { Write-Host "CMake configuration failed."; exit 1 }
    }
    cmake --build $buildDir
    if ($LASTEXITCODE -ne 0) { Write-Host "Build failed."; exit 1 }

    $found = Get-ChildItem -Path $buildDir -Recurse -Filter programa.exe -ErrorAction SilentlyContinue |
        Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if (-not $found) {
        Write-Host "programa.exe not found after build."
        exit 2
    }
    $Exe = $found.FullName
}
$Exe = (Resolve-Path $Exe).Path

function Invoke-Program([string]$exe, [string]$file) {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exe
    $psi.Arguments = '"' + $file + '"'
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false
    $psi.StandardOutputEncoding = [System.Text.Encoding]::UTF8
    $p = [System.Diagnostics.Process]::Start($psi)
    $errTask = $p.StandardError.ReadToEndAsync()
    $output = $p.StandardOutput.ReadToEnd()
    $p.WaitForExit()
    return $output
}

$ok = 0
$total = 0
foreach ($dir in $TestDirs) {
    $folder = Join-Path $root $dir
    if (-not (Test-Path $folder)) { continue }
    foreach ($file in (Get-ChildItem -Path $folder -Filter *.in | Sort-Object Name)) {
        $expectedPath = [System.IO.Path]::ChangeExtension($file.FullName, ".out")
        if (-not (Test-Path $expectedPath)) { continue }
        $total++
        $expected = [System.IO.File]::ReadAllText($expectedPath) -replace "`r`n", "`n"
        $actual = (Invoke-Program $Exe $file.FullName) -replace "`r`n", "`n"
        if ($actual -ceq $expected) {
            $ok++
            Write-Host "OK     $($file.Name)"
        } else {
            Write-Host "FAILED $($file.Name)"
        }
    }
}
Write-Host "$ok/$total tests passed"
if ($ok -ne $total) { exit 1 }
