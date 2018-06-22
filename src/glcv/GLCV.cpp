// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "GLCV.hpp"
#include "VulkanUtil.hpp"
#include "ErrorCheck.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <glcv/util/vector_util.hpp>

//#define DEBUG_PRINT(msg) {}
#define DEBUG_PRINT(msg) std::cout << "DEBUG: " << (msg) << std::endl

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT /*flags*/,
                                              VkDebugReportObjectTypeEXT /*obj_type*/,
                                              uint64_t /*obj*/,
                                              size_t /*location*/,
                                              int32_t /*code*/,
                                              const char * /*layer_prefix*/,
                                              const char *msg,
                                              void * /*user_data*/)
{
    std::cerr << "ERROR (Vulkan validation layer): " << msg << std::endl;
    return VK_FALSE;
}

// The 'vkCreateDebugReportCallbackEXT' function has to be looked up explicitly since it is an extension.
// This function handles the lookup process and calls the function if it exists.
VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                      const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugReportCallbackEXT *pCallback)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// The 'vkDestroyDebugReportCallbackEXT' function has to be looked up explicitly since it is an extension.
// This function handles the lookup process and calls the function if it exists.
void DestroyDebugReportCallbackEXT(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks *pAllocator)
{
    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));

    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

} // namespace

void GLCV::init(const std::string &app_name,
                const std::vector<const char *> &extension_names,
                const std::vector<const char *> &layer_names,
                bool set_debug_callback)
{
    GLCV::self().create_instance(app_name, extension_names, layer_names);

    if (set_debug_callback) {
        GLCV::self().setup_debug_callback();
    }
}

std::vector<VkPhysicalDevice> GLCV::get_available_devices()
{
    auto &self = GLCV::self();
    return glcv::get_available_devices(*self.instance_);
}

GLCV::GLCV() = default;
GLCV::~GLCV() = default;

GLCV &GLCV::self()
{
    static GLCV glcv;
    return glcv;
}

void GLCV::create_instance(const std::string &app_name,
                           const std::vector<const char *> &extension_names,
                           const std::vector<const char *> &layer_names)
{
    glcv::check_extension_support(extension_names);
    glcv::check_layer_support(layer_names);

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instance_info = {};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pNext = nullptr;
    instance_info.flags = 0;
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
    instance_info.ppEnabledExtensionNames = (extension_names.empty() ? nullptr : extension_names.data());
    instance_info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
    instance_info.ppEnabledLayerNames = (layer_names.empty() ? nullptr : layer_names.data());

    instance_ = std::shared_ptr<VkInstance>(new VkInstance(nullptr), [](auto p) {
        if (*p) {
            vkDestroyInstance(*p, nullptr);
            DEBUG_PRINT("Vulkan instance destroyed");
        }
        delete p;
    });

    GLCV_CHECK(vkCreateInstance(&instance_info, nullptr, instance_.get()));
    DEBUG_PRINT("Vulkan instance created");
}

void GLCV::setup_debug_callback()
{
    VkDebugReportCallbackCreateInfoEXT debug_info = {};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_info.pNext = nullptr;
    debug_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_info.pfnCallback = debug_callback;
    debug_info.pUserData = nullptr;

    debug_callback_ = std::shared_ptr<VkDebugReportCallbackEXT>(new VkDebugReportCallbackEXT(nullptr), [this](auto p) {
        if (*p) {
            DestroyDebugReportCallbackEXT(*instance_, *p, nullptr);
            DEBUG_PRINT("Vulkan debug report callback destroyed");
        }
        delete p;
    });

    GLCV_CHECK(CreateDebugReportCallbackEXT(*instance_, &debug_info, nullptr, debug_callback_.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
}

//} // namespace glcv
