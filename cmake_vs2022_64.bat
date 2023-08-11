set CMAKE="cmake.exe"
set BUILD_DIR="build_vs2022_64"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows-release -G "Visual Studio 17 2022" .. 
popd
