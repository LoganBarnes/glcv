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

vk::Result to_vk_result(vk::Result result)
{
    return result;
}

vk::Result to_vk_result(VkResult result)
{
    vk::Result vk_result = vk::Result::eSuccess;

    switch (result) {
    case VK_SUCCESS:
        return vk::Result::eSuccess;
    case VK_NOT_READY:
        return vk::Result::eNotReady;
    case VK_TIMEOUT:
        return vk::Result::eTimeout;
    case VK_EVENT_SET:
        return vk::Result::eEventSet;
    case VK_EVENT_RESET:
        return vk::Result::eEventReset;
    case VK_INCOMPLETE:
        return vk::Result::eIncomplete;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return vk::Result::eErrorOutOfHostMemory;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return vk::Result::eErrorOutOfDeviceMemory;
    case VK_ERROR_INITIALIZATION_FAILED:
        return vk::Result::eErrorInitializationFailed;
    case VK_ERROR_DEVICE_LOST:
        return vk::Result::eErrorDeviceLost;
    case VK_ERROR_MEMORY_MAP_FAILED:
        return vk::Result::eErrorMemoryMapFailed;
    case VK_ERROR_LAYER_NOT_PRESENT:
        return vk::Result::eErrorLayerNotPresent;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return vk::Result::eErrorExtensionNotPresent;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return vk::Result::eErrorFeatureNotPresent;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return vk::Result::eErrorIncompatibleDriver;
    case VK_ERROR_TOO_MANY_OBJECTS:
        return vk::Result::eErrorTooManyObjects;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return vk::Result::eErrorFormatNotSupported;
    case VK_ERROR_FRAGMENTED_POOL:
        return vk::Result::eErrorFragmentedPool;
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return vk::Result::eErrorOutOfPoolMemory;
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return vk::Result::eErrorInvalidExternalHandle;
    case VK_ERROR_SURFACE_LOST_KHR:
        return vk::Result::eErrorSurfaceLostKHR;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return vk::Result::eErrorNativeWindowInUseKHR;
    case VK_SUBOPTIMAL_KHR:
        return vk::Result::eSuboptimalKHR;
    case VK_ERROR_OUT_OF_DATE_KHR:
        return vk::Result::eErrorOutOfDateKHR;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return vk::Result::eErrorIncompatibleDisplayKHR;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return vk::Result::eErrorValidationFailedEXT;
    case VK_ERROR_INVALID_SHADER_NV:
        return vk::Result::eErrorInvalidShaderNV;
    case VK_ERROR_FRAGMENTATION_EXT:
        return vk::Result::eErrorFragmentationEXT;
    case VK_ERROR_NOT_PERMITTED_EXT:
        return vk::Result::eErrorNotPermittedEXT;
    case VK_RESULT_RANGE_SIZE:
    case VK_RESULT_MAX_ENUM:
        throw std::runtime_error("VK_RESULT_RANGE_SIZE and VK_RESULT_MAX_ENUM are not useful error enums");
    }
    return vk_result;
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
