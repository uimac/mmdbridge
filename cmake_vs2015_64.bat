set CMAKE="%VCPKG_DIR%\downloads\cmake-3.8.0-win32-x86\bin\cmake.exe"
set BUILD_DIR="build_vs2015_64"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows -G "Visual Studio 14 2015 Win64" .. 
popd
