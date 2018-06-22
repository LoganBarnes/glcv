// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include "VulkanUtil.hpp"

namespace glcv {

std::vector<VkExtensionProperties> get_available_extensions()
{
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    return extensions;
}

std::vector<VkLayerProperties> get_available_layers()
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, layers.data());
    return layers;
}

void check_extension_support(const std::vector<const char *> &requested)
{
    const std::vector<VkExtensionProperties> available = get_available_extensions();
    std::unordered_set<std::string> available_names(available.size());

    for (const auto &extension : available) {
        available_names.emplace(extension.extensionName);
    }

    for (const auto &extension : requested) {
        if (available_names.find(extension) == available_names.end()) {
            throw std::runtime_error("Extension '" + std::string(extension) + "' NOT SUPPORTED");
        }
    }
}

void check_layer_support(const std::vector<const char *> &requested)
{
    const std::vector<VkLayerProperties> available = get_available_layers();
    std::unordered_set<std::string> available_names(available.size());

    for (const auto &layer : available) {
        available_names.emplace(layer.layerName);
    }

    for (const auto &layer : requested) {
        if (available_names.find(layer) == available_names.end()) {
            throw std::runtime_error("Layer '" + std::string(layer) + "' NOT SUPPORTED");
        }
    }
}

} // namespace glcv
