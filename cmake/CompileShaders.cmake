#
# EXAMPLE USAGE:
#
#   include(CompileShaders)
#   compile_shaders(shader_target shader_output_dir shader1 shader2 shader3...)
#
#========================================================================================

function(compile_shaders target_name OUTPUT_DIR)

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR})

    set(SPV_FILES "")

    foreach (shader_file ${ARGN})
        get_filename_component(FILENAME_NO_EXT ${shader_file} NAME_WE)
        get_filename_component(FILE_EXT ${shader_file} EXT)
        string(REPLACE "." "_" FILE_EXT_NO_DOT ${FILE_EXT})

        set(OUTPUT_FILE ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/${FILENAME_NO_EXT}${FILE_EXT_NO_DOT}.spv)

        add_custom_command(OUTPUT "${OUTPUT_FILE}"
                COMMAND glslangValidator ${shader_file} -V -o "${OUTPUT_FILE}"
                DEPENDS ${shader_file}
                )

        list(APPEND SPV_FILES ${OUTPUT_FILE})
    endforeach ()

    add_library(${target_name} INTERFACE)
    target_include_directories(${target_name} INTERFACE ${CMAKE_BINARY_DIR})
    target_sources(${target_name} INTERFACE ${SPV_FILES})

endfunction()
