CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(mmdbridge) # .sln

# shared compiler options
set(CMAKE_CXX_FLAGS "/EHsc /wd4267"
    )
# embeded pdb 
set(CMAKE_CXX_FLAGS_DEBUG
    "/MDd /Od /ZI"
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