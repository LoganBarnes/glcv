// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCV.hpp>

#define GLCV_CHECK(func)                                                                                               \
    {                                                                                                                  \
        VkResult result = (func);                                                                                      \
        if (result != VK_SUCCESS) {                                                                                    \
            throw std::runtime_error("GLCV ERROR: " + glcv::to_string(result));                                        \
        }                                                                                                              \
    }

namespace glcv {

std::string to_string(VkResult &result);

} // namespace glcv
