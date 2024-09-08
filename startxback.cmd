<# :

:: Developed in collaboration with MAS & ASDCORP

@setlocal DisableDelayedExpansion
@echo off

echo "%*" | find /i "-el" >nul && set _elev=1

set _PSarg="""%~f0""" -el

setlocal EnableDelayedExpansion

>nul fltmc || (
if not defined _elev powershell "start cmd.exe -arg '/c \"!_PSarg:'=''!\"' -verb runas" && exit /b
echo.
echo This script require administrator privileges.
echo To do so, right click on this script and select 'Run as administrator'.
pause
exit /b
)

set "psScript=%~f0" & powershell -nop -c "Get-Content """$env:psScript""" -Raw | iex" & exit /b
: #>

function Patch-Exp($DLL) {
    $UserDLL = "$Env:LocalAppData\$DLL"
    $SystemDLL64 = "$Env:ProgramFiles\$DLL"
    $SystemDLL32 = "${Env:ProgramFiles(x86)}\$DLL"

    $Paths = @()

    if(Test-Path -Path $UserDLL) { $Paths += ,$UserDLL }
    if(Test-Path -Path $SystemDLL64) { $Paths += ,$SystemDLL64 }
    if(Test-Path -Path $SystemDLL32) { $Paths += ,$SystemDLL32 }

    foreach($Path in $Paths) {
        $Backup = "$Path.bak"
        if(Test-Path -Path $Backup) {
            Write-Host "$Path has already been patched! If you wish to restore the unpatched copy, delete $Path and rename $Path.bak to $Path"
        } else {
            Write-Host "Patching $Path"
            Copy-Item -Path $Path -Destination $Backup
            $Bytes = Get-Content $Path -Raw -Encoding Byte
            $String = $Bytes.ForEach('ToString', 'X2') -join ' '
            $String = $String -replace '\b52 53 41 31 00 04 00 00 03 00 00 00 80 00 00 00 00 00 00 00 00 00 00 00 01 00 01\b(.*)', '52 53 41 31 00 04 00 00 03 00 00 00 80 00 00 00 00 00 00 00 00 00 00 00 00 00 01$1'
            [byte[]]$ModifiedBytes = -split $String -replace '^', '0x'
            Set-Content -Path $Path -Value $ModifiedBytes -Encoding Byte
            Write-Host "Patched $Path"
        }
    }
}

function Install-License($registryPath) {
    New-Item $registryPath -ErrorAction SilentlyContinue | Out-Null
    Set-ItemProperty -Path $registryPath -Name "LicenseHash" -Value "deadbeefdeadbeefdeadbeefdeadbeef" -ErrorAction SilentlyContinue
    Set-ItemProperty -Path $registryPath -Name "ActivationData" -Value "deadbeefdeadbeefdeadbeefdeadbeef4e9934f69c3fd8c3e8502a2fd1ab89c2e78671d38a9b97ba313f5eaba6fd420f" -ErrorAction SilentlyContinue
    Write-Host "Installed license at $registryPath"
}

Write-Host "Killing processes"
cmd.exe /c taskkill /f /im explorer.exe | Out-Null
Stop-Process -Name StartIsBackCfg -ErrorAction SilentlyContinue
Stop-Process -Name StartAllBackCfg -ErrorAction SilentlyContinue
Sleep 1

Write-Host "Patching DLLs"
Patch-Exp "StartIsBack\StartIsBack32.dll"
Patch-Exp "StartIsBack\StartIsBack64.dll"
Patch-Exp "StartAllBack\StartAllBackX64.dll"

Write-Host "Installing licenses"
Install-License "HKCU:\Software\StartIsBack\License"
Install-License "HKLM:\Software\StartIsBack\License"

Write-Host "Done"
Start-Process explorer.exe
pause


