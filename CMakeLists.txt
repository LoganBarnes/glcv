###############################################################
# Vulkan Graphics Classes
# Copyright (c) 2018. All rights reserved.
###############################################################
cmake_minimum_required(VERSION 3.11.0)
project(VulkanGraphicsClasses)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VGC_BUILD_TESTS "Build unit tests with GoogleTest and GLFW" OFF)
option(VGC_BUILD_EXAMPLES "Build example classes with GLFW" OFF)
option(VGC_UPDATE_REPOS "Update thirdparty git repos" OFF)
option(VGC_USE_DEV_FLAGS "Compile with all the flags" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

######################
### EXTERNAL REPOS ###
######################
include(DownloadProject)

if (VGC_UPDATE_REPOS)
    set(UPDATE_STATUS 0)
else ()
    set(UPDATE_STATUS 1)
endif ()

### GLM ###
download_project(PROJ glm
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

add_definitions(-DGLM_FORCE_RADIANS -DGLM_ENABLE_EXPERIMENTAL)

### ImGui ###
download_project(PROJ imgui
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.60
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        ${QUIET_FLAG}
        )

find_package(Vulkan REQUIRED)

add_library(vgc-thirdparty INTERFACE)
target_include_directories(vgc-thirdparty SYSTEM INTERFACE ${glm_SOURCE_DIR})
target_link_libraries(vgc-thirdparty INTERFACE Vulkan::Vulkan)


#############################
### Project Configuration ###
#############################
if (VGC_USE_DEV_FLAGS)
    if (NOT MSVC)
        set(STRICT_FLAGS -Wall -Wextra -Werror -Wpedantic -Winit-self
                -Wold-style-cast -Woverloaded-virtual -Wsign-conversion
                -Wmissing-declarations -Wmissing-include-dirs -Wshadow)
    else ()
        set(STRICT_FLAGS -Wall -WX)
    endif ()
endif ()

if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS) # silly microsoft
endif ()

###############################
### Vulkan Graphics Classes ###
###############################
set(VGC_SOURCE
        src/vgc/VGC.cpp
        src/vgc/VGC.hpp
        )

add_library(vgc ${VGC_SOURCE})
target_include_directories(vgc PUBLIC src)
target_link_libraries(vgc vgc-thirdparty)
target_compile_options(vgc PRIVATE ${STRICT_FLAGS})

if (${VGC_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()

