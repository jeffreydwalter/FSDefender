@echo off
setlocal enabledelayedexpansion

echo Installing and starting FSDefender driver...

:: Check for administrator privileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo This script must be run as Administrator.
    echo Please right-click the script and select "Run as administrator".
    pause
    exit /b 1
)

:: Ensure we're in the correct directory
cd /d %~dp0

:: Install the driver
echo Installing the driver...
pnputil /add-driver x64\Release\FSD\FSD.inf /install
if %errorlevel% neq 0 (
    echo Failed to install the driver.
    pause
    exit /b 1
)

:: Start the driver
echo Starting the FSD service...
sc start FSD
if %errorlevel% neq 0 (
    echo Failed to start the FSD service.
    echo Checking service status...
    sc query FSD
) else (
    echo FSD service started successfully.
)

:: Verify the driver is running
echo Verifying driver status...
sc query FSD

:: Optional: Display some driver information
echo.
echo Driver Information:
driverquery | findstr FSD

:: Optional: Check Event Viewer for recent events
echo.
echo Recent Event Log entries (if any):
wevtutil qe System /q:"*[System[(EventID=7000 or EventID=7001) and TimeCreated[timediff(@SystemTime) <= 30000]]]" /f:text /c:1 /rd:true

echo.
echo Installation and startup process completed.
echo If you encountered any issues, please check the output above for error messages.

pause