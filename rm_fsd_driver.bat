@echo off
setlocal enabledelayedexpansion

echo Cleaning up FSDefender driver...

:: Stop the driver if it's running
echo Stopping FSD service...
sc stop FSD

:: Uninstall the driver
echo Uninstalling driver...
pnputil /delete-driver oem1.inf /uninstall /force

:: Remove the driver file
echo Removing driver file...
if exist C:\Windows\System32\drivers\FSD.sys (
    del /F C:\Windows\System32\drivers\FSD.sys
    if errorlevel 1 echo Warning: Could not delete FSD.sys. It may be removed after reboot.
)

:: Remove registry entries
echo Removing registry entries...
reg delete HKLM\SYSTEM\CurrentControlSet\Services\FSD /f

echo Cleanup complete. Please reboot the system before reinstalling the driver.
pause