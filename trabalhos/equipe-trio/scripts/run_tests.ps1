<#
  Compila (se necessario) e roda os testes (.in/.out) no Windows.
  Os fins de linha CRLF e LF sao normalizados antes da comparacao.

  Uso, a partir de trabalhos\equipe-trio:
    .\scripts\run_tests.ps1
    .\scripts\run_tests.ps1 -Exe .\build\Debug\programa.exe
#>
param(
    [string]$Exe = "",
    [string[]]$TestDirs = @("tests", "..\..\testes")
)

$root = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $root "build"

# --- Compilar (apenas quando -Exe nao for fornecido explicitamente) ---
if (-not $Exe) {
    if (-not (Test-Path $buildDir)) {
        Write-Host "Configurando CMake..."
        cmake -S $root -B $buildDir
        if ($LASTEXITCODE -ne 0) { Write-Host "Falha na configuracao do CMake."; exit 1 }
    }
    Write-Host "Compilando..."
    cmake --build $buildDir
    if ($LASTEXITCODE -ne 0) { Write-Host "Falha na compilacao."; exit 1 }

    $found = Get-ChildItem -Path $buildDir -Recurse -Filter programa.exe -ErrorAction SilentlyContinue |
        Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if (-not $found) {
        Write-Host "programa.exe nao encontrado apos compilacao."
        exit 2
    }
    $Exe = $found.FullName
}
$Exe = (Resolve-Path $Exe).Path

function Invoke-Programa([string]$exe, [string]$arquivo) {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exe
    $psi.Arguments = '"' + $arquivo + '"'
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false
    $psi.StandardOutputEncoding = [System.Text.Encoding]::UTF8
    $p = [System.Diagnostics.Process]::Start($psi)
    $errTask = $p.StandardError.ReadToEndAsync()
    $saida = $p.StandardOutput.ReadToEnd()
    $p.WaitForExit()
    return $saida
}

$ok = 0
$total = 0
foreach ($dir in $TestDirs) {
    $pasta = Join-Path $root $dir
    if (-not (Test-Path $pasta)) { continue }
    foreach ($arq in (Get-ChildItem -Path $pasta -Filter *.in | Sort-Object Name)) {
        $esperadoPath = [System.IO.Path]::ChangeExtension($arq.FullName, ".out")
        if (-not (Test-Path $esperadoPath)) { continue }
        $total++
        $esperado = [System.IO.File]::ReadAllText($esperadoPath) -replace "`r`n", "`n"
        $obtido = (Invoke-Programa $Exe $arq.FullName) -replace "`r`n", "`n"
        if ($obtido -ceq $esperado) {
            $ok++
            Write-Host "OK     $($arq.Name)"
        } else {
            Write-Host "FALHOU $($arq.Name)"
        }
    }
}
Write-Host "$ok/$total testes passaram"
if ($ok -ne $total) { exit 1 }
