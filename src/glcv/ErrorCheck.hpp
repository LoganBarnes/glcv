// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include "glcv/GLCV.hpp"

#define GLCV_CHECK(func)                                                                                               \
    {                                                                                                                  \
        vk::Result result = (func);                                                                                    \
        if (result != vk::Result::eSuccess) {                                                                          \
            throw std::runtime_error("GLCV ERROR: " + glcv::to_string(result));                                        \
        }                                                                                                              \
    }

namespace glcv {

std::string to_string(vk::Result &result);

void check_extension_support(const std::vector<const char *> &requested_extensions);
void check_layer_support(const std::vector<const char *> &requested_layers);

} // namespace glcv
