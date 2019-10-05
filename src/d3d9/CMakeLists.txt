
##############################################################################
# d3d9
##############################################################################
file(GLOB d3d9_sources 
    *.cpp
    *.h
    d3d9.rc
    hook.def
    )
source_group("src" FILES ${d3d9_sources})

include_directories(
    ../umbase
    ../MikuMikuFormats
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/include
    ${DXSDK_INCLUDE_DIRS}
    )
link_directories(
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/lib/Win32
    )
add_library(d3d9 SHARED
    ${d3d9_sources}
    )

target_link_libraries(d3d9 PRIVATE Shlwapi)
target_link_libraries(d3d9 PRIVATE WinMM)
target_link_libraries(d3d9 PRIVATE MMDExport)
target_link_libraries(d3d9 PRIVATE MikuMikuFormats)
target_link_libraries(d3d9 PRIVATE umbase)
target_link_libraries(d3d9 PRIVATE ${DXSDK_LIBRARIES}/d3d12.lib)
target_link_libraries(d3d9 PRIVATE ${DXSDK_LIBRARIES}/dxgi.lib)
target_link_libraries(d3d9 PRIVATE ${DXSDK_LIBRARIES}/D3DCompiler.lib)

install(TARGETS d3d9
    RUNTIME DESTINATION ${MMD_DIR}
    )

set_target_properties(
    d3d9 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
    ${MMD_DIR}
    )
