###############################################################
# Graphics Library Classes Vulkan
# Copyright (c) 2018. All rights reserved.
###############################################################

######################
### EXTERNAL REPOS ###
######################

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

############################
### Example Loop Library ###
############################
configure_file(ExampleConfig.hpp.in ${CMAKE_BINARY_DIR}/examples/ExampleConfig.hpp)

set(LOOP_SOURCE
        loop/SimpleLoop.cpp
        loop/SimpleLoop.hpp
        )

# Simple update/render loop for the examples
add_library(example_loop ${LOOP_SOURCE})
target_link_libraries(example_loop glcv glcv-examples-thirdparty)
target_include_directories(example_loop PUBLIC . ${CMAKE_BINARY_DIR})

################
### Examples ###
################
function(add_example example_dir)
    set(exec_name example_${example_dir})
    add_executable(${exec_name} ${example_dir}/main.cpp)
    target_link_libraries(${exec_name} example_loop)
    target_compile_options(${exec_name} PRIVATE ${SYSTEM_FLAGS} ${STRICT_FLAGS})
endfunction()

add_example(0_basics)

file(GLOB_RECURSE SHADER_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*
        )

compile_shaders(shaders examples/spirv ${SHADER_FILES})

target_link_libraries(example_loop shaders)
