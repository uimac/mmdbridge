@echo off
for /F "usebackq" %%i in (`forfiles /P "%VCPKG_DIR%\downloads" /m cmake* /c "cmd /c if @isdir==TRUE echo @path"`) do set CMAKE_DIR=%%i
@echo on

set CMAKE="%CMAKE_DIR%\bin\cmake.exe"
set BUILD_DIR="build_vs2015_64"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows -G "Visual Studio 14 2015 Win64" .. 
popd
