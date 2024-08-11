@echo off
setlocal EnableDelayedExpansion

REM Define certificate details
set CertName=FSDefender Test Certificate
set CertFile=FSDefenderTestCert
set Password=password

REM Remove old certificates with the same name from the certificate stores
powershell -ExecutionPolicy Bypass -Command "Get-ChildItem -Path Cert:\LocalMachine\My, Cert:\LocalMachine\Root, Cert:\LocalMachine\TrustedPublisher | Where-Object {$_.Subject -eq 'CN=%CertName%'} | Remove-Item -Force"
if %errorlevel% neq 0 echo Error removing old certificates & exit /b %errorlevel%

REM Generate a self-signed certificate and export to PFX
powershell -ExecutionPolicy Bypass -Command "New-SelfSignedCertificate -Subject 'CN=%CertName%' -CertStoreLocation 'Cert:\LocalMachine\My' -KeyAlgorithm RSA -KeyLength 2048 -Provider 'Microsoft Enhanced RSA and AES Cryptographic Provider' -KeyExportPolicy Exportable -KeyUsage DigitalSignature -Type CodeSigningCert | Export-PfxCertificate -FilePath %CertFile%.pfx -Password (ConvertTo-SecureString -String '%Password%' -Force -AsPlainText)"
if %errorlevel% neq 0 echo Error generating certificate & exit /b %errorlevel%

REM Check if the PFX file was created successfully
if not exist %CertFile%.pfx (
    echo Error: PFX file not found.
    exit /b 1
)

REM Export the certificate to a .cer file
powershell -ExecutionPolicy Bypass -Command "$cert = Get-ChildItem -Path Cert:\LocalMachine\My | Where-Object {$_.Subject -eq 'CN=%CertName%'} | Select-Object -First 1; if ($cert) { Export-Certificate -Cert $cert -FilePath %CertFile%.cer } else { Write-Error 'Certificate not found' }"
if %errorlevel% neq 0 echo Error exporting certificate & exit /b %errorlevel%

REM Check if the CER file was created successfully
if not exist %CertFile%.cer (
    echo Error: CER file not found. Attempting to use PFX file instead.
    set CertToUse=%CertFile%.pfx
) else (
    set CertToUse=%CertFile%.cer
)

REM Install the certificate into the Trusted Root Certification Authorities store
powershell -ExecutionPolicy Bypass -Command "Import-Certificate -FilePath !CertToUse! -CertStoreLocation Cert:\LocalMachine\Root"
if %errorlevel% neq 0 echo Error importing to Root store & exit /b %errorlevel%

powershell -ExecutionPolicy Bypass -Command "Import-Certificate -FilePath !CertToUse! -CertStoreLocation Cert:\LocalMachine\TrustedPublisher"
if %errorlevel% neq 0 echo Error importing to TrustedPublisher store & exit /b %errorlevel%

REM Define the path to your driver files
set DriverPath=x64\Release\FSD

REM Sign the driver files
signtool sign /v /f %CertFile%.pfx /p %Password% /fd SHA256 /t http://timestamp.digicert.com !DriverPath!\FSD.sys
if %errorlevel% neq 0 echo Error signing FSD.sys & exit /b %errorlevel%

signtool sign /v /f %CertFile%.pfx /p %Password% /fd SHA256 /t http://timestamp.digicert.com !DriverPath!\fsd.cat
if %errorlevel% neq 0 echo Error signing fsd.cat & exit /b %errorlevel%

echo Driver signing completed successfully.
endlocal
pause