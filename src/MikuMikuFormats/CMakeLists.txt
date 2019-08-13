file(GLOB MikuMikuFormatsSources
    *.cpp
    *.h
    )
source_group("src" FILES ${MikuMikuFormatsSources})
add_library(MikuMikuFormats
    ${MikuMikuFormatsSources}
    )

