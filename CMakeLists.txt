###############################################################
# Graphics Library Classes Vulkan
# Copyright (c) 2018. All rights reserved.
###############################################################
cmake_minimum_required(VERSION 3.12)
project(GLCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GLCV_BUILD_TESTS "Build unit tests with GoogleTest and GLFW" OFF)
option(GLCV_BUILD_EXAMPLES "Build example classes with GLFW" OFF)
option(GLCV_UPDATE_REPOS "Update thirdparty git repos" OFF)
option(GLCV_USE_DEV_FLAGS "Compile with all the flags" OFF)
option(GLCV_HEADLESS "Run examples without a window" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

######################
### EXTERNAL REPOS ###
######################
include(DownloadProject)
include(CompileShaders)

if (GLCV_UPDATE_REPOS)
    set(UPDATE_STATUS 0)
else ()
    set(UPDATE_STATUS 1)
endif ()

### GLM ###
#download_project(PROJ glm
#        PREFIX thirdparty
#        GIT_REPOSITORY https://github.com/g-truc/glm.git
#        GIT_TAG 0.9.9.0
#        UPDATE_DISCONNECTED ${UPDATE_STATUS}
#        QUIET
#        )

find_package(Vulkan REQUIRED)

add_library(glcv_thirdparty INTERFACE)
#target_include_directories(glcv_thirdparty SYSTEM INTERFACE ${glm_SOURCE_DIR})
target_link_libraries(glcv_thirdparty INTERFACE Vulkan::Vulkan)
#target_compile_definitions(glcv_thirdparty
#        INTERFACE -DGLM_FORCE_RADIANS
#        INTERFACE -DGLM_ENABLE_EXPERIMENTAL
#        INTERFACE -DGLM_FORCE_EXPLICIT_CTOR
#        )

#############################
### Project Configuration ###
#############################
if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS) # silly microsoft
    set(SYSTEM_FLAGS /experimental:external /external:anglebrackets /external:W0)
endif ()

if (GLCV_USE_DEV_FLAGS)
    if (NOT MSVC)
        set(STRICT_FLAGS -Wall -Wextra -Werror -Wpedantic -Winit-self
                -Wold-style-cast -Woverloaded-virtual -Wsign-conversion
                -Wmissing-declarations -Wmissing-include-dirs -Wshadow)
    else ()
        set(STRICT_FLAGS /WX)
    endif ()
endif ()

###############################
### Vulkan Graphics Classes ###
###############################
file(GLOB_RECURSE GLCV_SOURCE_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*
        )

add_library(glcv ${GLCV_SOURCE_FILES})
target_include_directories(glcv PUBLIC src)
target_link_libraries(glcv glcv_thirdparty)
target_compile_options(glcv PRIVATE ${SYSTEM_FLAGS} ${STRICT_FLAGS})

if (${GLCV_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()

