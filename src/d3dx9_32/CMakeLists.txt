INCLUDE_DIRECTORIES(
    $ENV{DXSDK_DIR}/include
    )
ADD_DEFINITIONS(
    -DNOMINMAX
    -DUNICODE
    -D_UNICODE
    )
LINK_DIRECTORIES(
    $ENV{DXSDK_DIR}/lib/x64
    )
ADD_LIBRARY(d3dx9_32 SHARED
    d3dx9.cpp
    d3dx9.h
    hook64.def
    )
TARGET_LINK_LIBRARIES(d3dx9_32 
    Shlwapi
    )
INSTALL(TARGETS d3dx9_32
    RUNTIME DESTINATION ${MMD_DIR}
    )

