###############################################################
# Graphics Library Classes Vulkan
# Copyright (c) 2018. All rights reserved.
###############################################################
cmake_minimum_required(VERSION 3.11.0)
project(GLCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(BUILD_SHARED_LIBS ON)

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

if (GLCV_UPDATE_REPOS)
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

#set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# Vulkan is not guaranteed to link statically
#set(BUILD_SHARED_LIBS ON)
find_package(Vulkan REQUIRED)
#set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})

message(WARNING "Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")
#get_target_property(Vulkan_LIBS Vulkan::Vulkan INTERFACE_LINK_LIBRARIES)
get_target_property(Vulkan_LIBS Vulkan::Vulkan IMPORTED_LOCATION)
message(WARNING "Vulkan_LIBS: ${Vulkan_LIBS}")

add_library(glcv-thirdparty INTERFACE)
target_include_directories(glcv-thirdparty SYSTEM INTERFACE ${glm_SOURCE_DIR})
#target_link_libraries(glcv-thirdparty INTERFACE Vulkan::Vulkan)
target_include_directories(glcv-thirdparty SYSTEM INTERFACE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(glcv-thirdparty INTERFACE ${Vulkan_LIBRARIES})


#############################
### Project Configuration ###
#############################
if (GLCV_USE_DEV_FLAGS)
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
set(GLCV_SOURCE
        src/glcv/detail/VulkanExt.cpp
        src/glcv/detail/VulkanExt.hpp

        src/glcv/ErrorCheck.cpp
        src/glcv/ErrorCheck.hpp
        src/glcv/GLCV.cpp
        src/glcv/GLCV.hpp
        src/glcv/VulkanUtil.cpp
        src/glcv/VulkanUtil.hpp

        src/glcv/util/vector_util.hpp
        )

add_library(glcv ${GLCV_SOURCE})
target_include_directories(glcv PUBLIC src)
target_link_libraries(glcv glcv-thirdparty)
target_compile_options(glcv PRIVATE ${STRICT_FLAGS})

if (${GLCV_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()

