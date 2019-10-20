
    file(GLOB ShaderSrc
        "*.h"
        "*.hlsl"
    )

    # VC filter
    source_group("src" FILES ${ShaderSrc})

    # exclude from build
    set_source_files_properties(${ShaderSrc} PROPERTIES VS_TOOL_OVERRIDE "None")
    set_source_files_properties(${ShaderSrc} PROPERTIES HEADER_FILE_ONLY "TRUE")

    add_custom_target(shader ALL
        SOURCES ${ShaderSrc})

    install(FILES ${ShaderSrc} DESTINATION ${MMD_DIR}/shader
    )
