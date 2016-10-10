:: Usage:
:: call find-in-path.bat [reinstall|clean]

:: Initialize environment
call "%~dp0env.bat"
if errorlevel 1 goto error_env

set EnvMSYSPath=%EnvRootPath%\msys

call "%~dp0tools\prepare-msys.bat" %1
exit /B %ERRORLEVEL%

:error_env
echo Failed to initialize environment.
endlocal
exit /B 1
