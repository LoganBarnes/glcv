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

std::vector<vk::ExtensionProperties> get_available_extensions()
{
    uint32_t extension_count = 0;
    GLCV_CHECK(vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));

    std::vector<vk::ExtensionProperties> extensions(extension_count);
    GLCV_CHECK(vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()));
    return extensions;
}

std::vector<vk::LayerProperties> get_available_layers()
{
    uint32_t layer_count = 0;
    GLCV_CHECK(vk::enumerateInstanceLayerProperties(&layer_count, nullptr));

    std::vector<vk::LayerProperties> layers(layer_count);
    GLCV_CHECK(vk::enumerateInstanceLayerProperties(&layer_count, layers.data()));
    return layers;
}

void check_extension_support(const std::vector<const char *> &requested)
{
    const std::vector<vk::ExtensionProperties> available = get_available_extensions();
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
    const std::vector<vk::LayerProperties> available = get_available_layers();
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

std::vector<vk::PhysicalDevice> get_available_devices(vk::Instance instance)
{
    uint32_t device_count = 0;
    GLCV_CHECK(instance.enumeratePhysicalDevices(&device_count, nullptr));

    std::vector<vk::PhysicalDevice> devices(device_count);
    GLCV_CHECK(instance.enumeratePhysicalDevices(&device_count, devices.data()));
    return devices;
}

} // namespace glcv
