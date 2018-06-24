// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "ErrorCheck.hpp"
#include <unordered_set>

namespace glcv {

std::string to_string(vk::Result &result)
{
    switch (result) {
    case vk::Result::eSuccess:
        return "Success";
    case vk::Result::eNotReady:
        return "NotReady";
    case vk::Result::eTimeout:
        return "Timeout";
    case vk::Result::eEventSet:
        return "EventSet";
    case vk::Result::eEventReset:
        return "EventReset";
    case vk::Result::eIncomplete:
        return "Incomplete";
    case vk::Result::eErrorOutOfHostMemory:
        return "ErrorOutOfHostMemory";
    case vk::Result::eErrorOutOfDeviceMemory:
        return "ErrorOutOfDeviceMemory";
    case vk::Result::eErrorInitializationFailed:
        return "ErrorInitializationFailed";
    case vk::Result::eErrorDeviceLost:
        return "ErrorDeviceLost";
    case vk::Result::eErrorMemoryMapFailed:
        return "ErrorMemoryMapFailed";
    case vk::Result::eErrorLayerNotPresent:
        return "ErrorLayerNotPresent";
    case vk::Result::eErrorExtensionNotPresent:
        return "ErrorExtensionNotPresent";
    case vk::Result::eErrorFeatureNotPresent:
        return "ErrorFeatureNotPresent";
    case vk::Result::eErrorIncompatibleDriver:
        return "ErrorIncompatibleDriver";
    case vk::Result::eErrorTooManyObjects:
        return "ErrorTooManyObjects";
    case vk::Result::eErrorFormatNotSupported:
        return "ErrorFormatNotSupported";
    case vk::Result::eErrorFragmentedPool:
        return "ErrorFragmentedPool";
    case vk::Result::eErrorOutOfPoolMemory:
        return "ErrorOutOfPoolMemory";
    case vk::Result::eErrorInvalidExternalHandle:
        return "ErrorInvalidExternalHandle";
    case vk::Result::eErrorSurfaceLostKHR:
        return "ErrorSurfaceLostKHR";
    case vk::Result::eErrorNativeWindowInUseKHR:
        return "ErrorNativeWindowInUseKHR";
    case vk::Result::eSuboptimalKHR:
        return "SuboptimalKHR";
    case vk::Result::eErrorOutOfDateKHR:
        return "ErrorOutOfDateKHR";
    case vk::Result::eErrorIncompatibleDisplayKHR:
        return "ErrorIncompatibleDisplayKHR";
    case vk::Result::eErrorValidationFailedEXT:
        return "ErrorValidationFailedEXT";
    case vk::Result::eErrorInvalidShaderNV:
        return "ErrorInvalidShaderNV";
    case vk::Result::eErrorFragmentationEXT:
        return "ErrorFragmentationEXT";
    case vk::Result::eErrorNotPermittedEXT:
        return "ErrorNotPermittedEXT";
    }
    return "Unknown Error";
}

void check_extension_support(const std::vector<const char *> &requested)
{
    const std::vector<vk::ExtensionProperties> available = vk::enumerateInstanceExtensionProperties();
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
    const std::vector<vk::LayerProperties> available = vk::enumerateInstanceLayerProperties();
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
