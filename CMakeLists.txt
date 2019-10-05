CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(mmdbridge) # .sln

# for UTF8 source codes
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Windows SDK
if("${SHORT_VERSION}" STREQUAL "10.0")
  message(STATUS "Targeting Windows 10. Setting Extensions to version ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
  set_property(TARGET ${EXE_NAME} PROPERTY VS_DESKTOP_EXTENSIONS_VERSION "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
endif()

# DirectX12
set(DXSDK_LIBRARIES "C:/Program\ Files\ (x86)/Windows\ Kits/10/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x64")
set(DXSDK_INCLUDE_DIRS "C:/Program\ Files\ (x86)/Windows\ Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um")

# shared compiler options
set(CMAKE_CXX_FLAGS "/EHsc /wd4267"
    )
# embeded pdb 
set(CMAKE_CXX_FLAGS_DEBUG
    "/MDd /Od /ZI"
    )

add_definitions(
    -DNOMINMAX
    -DUNICODE
    -D_UNICODE
    )

# path settings
get_filename_component(MMD_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/MikuMikuDance_x64
    ABSOLUTE
    )

# projects
add_subdirectory(src)

# additional install files
install(FILES
    DESTINATION ${MMD_DIR}
    )

install(DIRECTORY 
    DESTINATION ${MMD_DIR}/out)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)