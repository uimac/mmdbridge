ADD_SUBDIRECTORY(MikuMikuFormats)
ADD_SUBDIRECTORY(d3dx9_32)

##############################################################################
# d3d9
##############################################################################
FILE(GLOB d3d9_sources 
    d3d9/*.cpp
    d3d9/*.h
    d3d9/d3d9.rc
    d3d9/hook.def
    umbase/*.cpp
    umbase/*.h
    )
INCLUDE_DIRECTORIES(
    umbase
    d3dx9_32
    MikuMikuFormats
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/include
    ${ilmbase_INCLUDE_DIRS}
    ${Alembic_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
    )
LINK_DIRECTORIES(
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/lib/x64
    )
ADD_DEFINITIONS(
    -DNOMINMAX
    -DUNICODE
    -D_UNICODE
    -DWITH_VMD
    -DWITH_PMX
    -DWITH_ALEMBIC
    )
ADD_LIBRARY(d3d9 SHARED
    ${d3d9_sources}
    )
TARGET_LINK_LIBRARIES(d3d9 
    ${PYTHON_LIBRARIES}
    Shlwapi
    WinMM
    d3dx9_32
    MMDExport
    MikuMikuFormats
    ${ALEMBIC_LIBRARIES}
    )
INSTALL(TARGETS d3d9
    RUNTIME DESTINATION ${MMD_DIR}
    )

