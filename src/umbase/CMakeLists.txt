
##############################################################################
# umbase
##############################################################################
file(GLOB umbase_sources 
    *.cpp
    *.h
    )

source_group("src" FILES ${umbase_sources})

add_library(umbase STATIC
    ${umbase_sources}
    )