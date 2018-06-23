// ////////////////////////////////////////////////////////////
// Created on 6/23/18.
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <iostream>
#include "VulkanExt.hpp"

extern "C" {

static PFN_vkDestroyInstance pfn_vkDestroyInstance;
static PFN_vkCreateDebugReportCallbackEXT pfn_vkCreateDebugReportCallbackEXT;
static PFN_vkDestroyDebugReportCallbackEXT pfn_vkDestroyDebugReportCallbackEXT;
static PFN_vkDebugReportMessageEXT pfn_vkDebugReportMessageEXT;
void vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator)
{
    auto cpy1 = pfn_vkDestroyInstance;
    auto cpy2 = pfn_vkCreateDebugReportCallbackEXT;
    auto cpy3 = pfn_vkDestroyDebugReportCallbackEXT;
    auto cpy4 = pfn_vkDebugReportMessageEXT;
    std::cout << cpy1 << std::endl;
    std::cout << cpy2 << std::endl;
    std::cout << cpy3 << std::endl;
    std::cout << cpy4 << std::endl;
    pfn_vkDestroyInstance(instance, pAllocator);
}

VkResult vkCreateDebugReportCallbackEXT(VkInstance instance,
                                        const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                        const VkAllocationCallbacks *pAllocator,
                                        VkDebugReportCallbackEXT *pCallback)
{
    return pfn_vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

void vkDestroyDebugReportCallbackEXT(VkInstance instance,
                                     VkDebugReportCallbackEXT callback,
                                     const VkAllocationCallbacks *pAllocator)
{
    pfn_vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

void vkDebugReportMessageEXT(VkInstance instance,
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
    pfn_vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(instance.getProcAddr("vkDestroyInstance"));

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
