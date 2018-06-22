// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCV.hpp>
#include "ErrorCheck.hpp"

namespace glcv {

// TODO: Figure out how to do this properly
template <typename T, typename... Args>
std::vector<T> enumerate(VkResult (*vkEnumerate)(Args..., uint32_t *, T *), Args... args)
{
    uint32_t count = 0;
    GLCV_CHECK(vkEnumerate(args..., &count, nullptr));

    std::vector<T> data(count);
    GLCV_CHECK(vkEnumerate(args..., &count, data.data()));
    return data;
}

std::vector<VkExtensionProperties> get_available_extensions();
std::vector<VkLayerProperties> get_available_layers();

void check_extension_support(const std::vector<const char *> &requested_extensions);
void check_layer_support(const std::vector<const char *> &requested_layers);

std::vector<VkPhysicalDevice> get_available_devices(VkInstance instance);

} // namespace glcv
