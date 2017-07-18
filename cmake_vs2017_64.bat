@echo off
FOR /F "TOKENS=1,2,*" %%A IN ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v "15.0"') DO IF "%%A"=="15.0" SET VSPATH=%%C
@echo on

set CMAKE="%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set BUILD_DIR="build_vs2017_64"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows -G "Visual Studio 15 2017 Win64" .. 
popd
