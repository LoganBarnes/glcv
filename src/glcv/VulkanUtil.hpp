// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCV.hpp>

namespace glcv {

std::vector<VkExtensionProperties> get_available_extensions();
std::vector<VkLayerProperties> get_available_layers();

void check_extension_support(const std::vector<const char *> &requested_extensions);
void check_layer_support(const std::vector<const char *> &requested_layers);

} // namespace glcv
