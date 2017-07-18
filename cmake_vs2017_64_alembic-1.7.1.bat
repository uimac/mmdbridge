@echo off
FOR /F "TOKENS=1,2,*" %%A IN ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v "15.0"') DO IF "%%A"=="15.0" SET VSPATH=%%C
@echo on

set CMAKE="%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set MSBUILD="%VSPATH%\MSBuild\15.0\Bin\msbuild.exe"
set BUILD_DIR="build_vs2017_64_alembic"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows -D USE_HDF5=ON -D USE_STATIC_HDF5=ON -D HDF5_ROOT=%VCPKG_DIR%/installed/x64-windows/share/hdf5 -D ALEMBIC_DIR=%VCPKG_DIR%/installed/x64-windows/lib/cmake/Alembic -G "Visual Studio 15 2017 Win64" ../alembic-1.7.1 

%MSBUILD% /p:Configuration=Release /t:Build INSTALL.vcxproj
%MSBUILD% /p:Configuration=Debug /t:Build INSTALL.vcxproj

popd

