// ////////////////////////////////////////////////////////////
// Created on 6/23/18.
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "VulkanExt.hpp"
#include <iostream>

extern "C" {
static PFN_vkCreateDebugReportCallbackEXT pfn_vkCreateDebugReportCallbackEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance,
                                                              const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                                              const VkAllocationCallbacks *pAllocator,
                                                              VkDebugReportCallbackEXT *pCallback)
{
    return pfn_vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

static PFN_vkDestroyDebugReportCallbackEXT pfn_vkDestroyDebugReportCallbackEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance,
                                                           VkDebugReportCallbackEXT callback,
                                                           const VkAllocationCallbacks *pAllocator)
{
    pfn_vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

static PFN_vkDebugReportMessageEXT pfn_vkDebugReportMessageEXT;
VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(VkInstance instance,
                                                   VkDebugReportFlagsEXT flags,
                                                   VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t object,
                                                   size_t location,
                                                   int32_t messageCode,
                                                   const char *pLayerPrefix,
                                                   const char *pMessage)
{
    pfn_vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

vk::Result vkExtInitInstance(vk::Instance instance)
{
    pfn_vkCreateDebugReportCallbackEXT
        = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance.getProcAddr("vkCreateDebugReportCallbackEXT"));

    pfn_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
        instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

    pfn_vkDebugReportMessageEXT
        = reinterpret_cast<PFN_vkDebugReportMessageEXT>(instance.getProcAddr("vkDebugReportMessageEXT"));

    if (pfn_vkCreateDebugReportCallbackEXT == nullptr || pfn_vkDestroyDebugReportCallbackEXT == nullptr
        || pfn_vkDebugReportMessageEXT == nullptr) {
        return vk::Result::eErrorExtensionNotPresent;
    }
    return vk::Result::eSuccess;
}

} // extern "C"
