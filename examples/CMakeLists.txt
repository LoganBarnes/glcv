###############################################################
# Graphics Library Classes Vulkan
# Copyright (c) 2018. All rights reserved.
###############################################################
configure_file(ExampleConfig.hpp.in ${CMAKE_BINARY_DIR}/loop/ExampleConfig.hpp)

if (NOT GLCV_HEADLESS)
    ### GLFW ###
    download_project(PROJ glfw
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
            UPDATE_DISCONNECTED ${UPDATE_STATUS}
            QUIET
            )

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    # compile glfw with current project
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

    ### ImGui ###
    download_project(PROJ imgui
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG v1.60
            UPDATE_DISCONNECTED ${UPDATE_STATUS}
            ${QUIET_FLAG}
            )

    add_library(glcv-examples-thirdparty
            # imgui
            ${imgui_SOURCE_DIR}/imconfig.h
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui.h
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_internal.h
            ${imgui_SOURCE_DIR}/stb_rect_pack.h
            ${imgui_SOURCE_DIR}/stb_textedit.h
            ${imgui_SOURCE_DIR}/stb_truetype.h
            ${imgui_SOURCE_DIR}/examples/vulkan_example/imgui_impl_glfw_vulkan.cpp
            ${imgui_SOURCE_DIR}/examples/vulkan_example/imgui_impl_glfw_vulkan.h
            )
    target_include_directories(glcv-examples-thirdparty SYSTEM PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/examples/vulkan_example
            )
    target_link_libraries(glcv-examples-thirdparty Vulkan::Vulkan glfw)

    # Simple update/render loop for the examples
    add_library(example_loop loop/GLFWLoop.cpp loop/SimpleLoop.hpp)
    target_link_libraries(example_loop glcv glcv-examples-thirdparty)
    target_include_directories(example_loop PUBLIC . ${CMAKE_BINARY_DIR})
else ()
    add_library(example_loop loop/HeadlessLoop.cpp loop/SimpleLoop.hpp)
    target_link_libraries(example_loop glcv)
    target_include_directories(example_loop PUBLIC . ${CMAKE_BINARY_DIR})
endif ()


function(add_example example_dir)
    set(exec_name example_${example_dir})
    add_executable(${exec_name} ${example_dir}/main.cpp)
    target_link_libraries(${exec_name} example_loop)
    target_compile_options(${exec_name} PRIVATE ${STRICT_FLAGS})
endfunction()

# Examples
add_example(wire_cube)
add_example(wire_cube_camera)
