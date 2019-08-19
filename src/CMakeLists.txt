add_subdirectory(MikuMikuFormats)

##############################################################################
# d3d9
##############################################################################
file(GLOB d3d9_sources 
    d3d9/*.cpp
    d3d9/*.h
    d3d9/d3d9.rc
    d3d9/hook.def
    )
source_group("src" FILES ${d3d9_sources})

file(GLOB umbase_sources 
    umbase/*.cpp
    umbase/*.h
    )
source_group("src" FILES ${umbase_sources})

include_directories(
    umbase
    MikuMikuFormats
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/include
    )
link_directories(
    ${MMD_DIR}/Data
    $ENV{DXSDK_DIR}/lib/Win32
    )
add_definitions(
    -DNOMINMAX
    -DUNICODE
    -D_UNICODE
    )
add_library(d3d9 SHARED
    ${d3d9_sources}
    )
add_library(umbase STATIC
    ${umbase_sources}
    )
target_link_libraries(d3d9 
    Shlwapi
    WinMM
    MMDExport
    MikuMikuFormats
    umbase
    )
install(TARGETS d3d9
    RUNTIME DESTINATION ${MMD_DIR}
    )


set_target_properties(
    d3d9 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
    ${MMD_DIR}
    )
