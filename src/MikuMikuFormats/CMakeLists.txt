FILE(GLOB MikuMikuFormatsSources
    *.cpp
    *.h
    )
ADD_LIBRARY(MikuMikuFormats
    ${MikuMikuFormatsSources}
    )

