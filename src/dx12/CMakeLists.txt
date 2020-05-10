##############################################################################
# dx12
##############################################################################

file(GLOB dx12_sources 
    *.cpp
    *.h
    )
source_group("src" FILES ${dx12_sources})

include_directories(
    ../umbase
    ${DXSDK_INCLUDE_DIRS}
    )
add_library(dx12 SHARED
    ${dx12_sources}
    )

target_compile_definitions(dx12 PRIVATE DLL_EXPORTS=1)

target_link_libraries(dx12 PRIVATE ${DXSDK_LIBRARIES}/d3d9.lib)
target_link_libraries(dx12 PRIVATE ${DXSDK_LIBRARIES}/d3d12.lib)
target_link_libraries(dx12 PRIVATE ${DXSDK_LIBRARIES}/dxgi.lib)
target_link_libraries(dx12 PRIVATE ${DXSDK_LIBRARIES}/D3DCompiler.lib)

install(TARGETS dx12
    RUNTIME DESTINATION ${MMD_DIR}
    )

add_subdirectory(shader)