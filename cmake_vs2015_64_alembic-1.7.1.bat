set CMAKE="%VCPKG_DIR%\downloads\cmake-3.8.0-win32-x86\bin\cmake.exe"

@echo off
FOR /F "TOKENS=1,2,*" %%A IN ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\MSBuild\ToolsVersions\14.0" /v "MSBuildToolsPath"') DO IF "%%A"=="MSBuildToolsPath" SET MSBUILDPATH=%%C
@echo on

set MSBUILD="%MSBUILDPATH%msbuild.exe"
set BUILD_DIR="build_vs2015_64_alembic"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -D CMAKE_INSTALL_PREFIX=%VCPKG_DIR%/installed/x64-windows -D USE_HDF5=ON -D HDF5_ROOT=%VCPKG_DIR%/installed/x64-windows -G "Visual Studio 14 2015 Win64" ../alembic-1.7.1

%MSBUILD% /p:Configuration=Release /t:Build /m INSTALL.vcxproj
@rem %MSBUILD% /p:Configuration=Debug /t:Build /m INSTALL.vcxproj

popd
