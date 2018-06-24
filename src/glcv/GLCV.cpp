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
#include <glcv/detail/VulkanExt.hpp>

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

} // namespace

namespace glcv {

namespace detail {

GLCV::GLCV(const std::string &app_name,
           const std::vector<const char *> &extension_names,
           const std::vector<const char *> &layer_names,
           bool set_debug_callback)
{
    instance_ = make_shared_instance(app_name, extension_names, layer_names);

    if (set_debug_callback) {
        debug_report_callback_ = make_shared_debug_report_callback();
    }

    physical_device_ = make_shared_physical_device();
}

const vk::Instance &GLCV::instance() const
{
    return *instance_;
}

const vk::PhysicalDevice &GLCV::physical_device() const
{
    return *physical_device_;
}

std::shared_ptr<vk::Instance> GLCV::make_shared_instance(const std::string &app_name,
                                                         const std::vector<const char *> &extension_names,
                                                         const std::vector<const char *> &layer_names)
{
    glcv::check_extension_support(extension_names);
    glcv::check_layer_support(layer_names);

    const auto app_info = vk::ApplicationInfo()
                              .setPNext(nullptr)
                              .setPApplicationName(app_name.c_str())
                              .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                              .setPEngineName("No Engine")
                              .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                              .setApiVersion(VK_API_VERSION_1_0);

    const auto instance_info = vk::InstanceCreateInfo()
                                   .setPNext(nullptr)
                                   .setPApplicationInfo(&app_info)
                                   .setEnabledExtensionCount(static_cast<uint32_t>(extension_names.size()))
                                   .setPpEnabledExtensionNames(extension_names.data())
                                   .setEnabledLayerCount(static_cast<uint32_t>(layer_names.size()))
                                   .setPpEnabledLayerNames(layer_names.data());

    auto instance = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr), [](auto p) {
        if (*p) {
            p->destroy(nullptr);
            DEBUG_PRINT("Vulkan instance destroyed");
        }
        delete p;
    });

    GLCV_CHECK(vk::createInstance(&instance_info, nullptr, instance.get()));
    DEBUG_PRINT("Vulkan instance created");
    return instance;
}

std::shared_ptr<vk::DebugReportCallbackEXT> GLCV::make_shared_debug_report_callback()
{
    vk::Instance instance = *instance_;
    GLCV_CHECK(vkExtInitInstance(instance));

    const auto debug_info = vk::DebugReportCallbackCreateInfoEXT()
                                .setPNext(nullptr)
                                .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning)
                                .setPfnCallback(debug_callback)
                                .setPUserData(nullptr);

    auto debug_report_callback
        = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr), [instance](auto p) {
              if (*p) {
                  instance.destroy(*p, nullptr);
                  DEBUG_PRINT("Vulkan debug report callback destroyed");
              }
              delete p;
          });

    GLCV_CHECK(instance.createDebugReportCallbackEXT(&debug_info, nullptr, debug_report_callback.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
    return debug_report_callback;
}

std::shared_ptr<vk::PhysicalDevice> GLCV::make_shared_physical_device()
{
    std::vector<vk::PhysicalDevice> devices = instance_->enumeratePhysicalDevices();

    if (devices.empty()) {
        throw std::runtime_error("GLCV ERROR: No suitable GPU found!");
    }

    return std::make_shared<vk::PhysicalDevice>(devices.front());
}

} // namespace detail

GLCV make_glcv(const std::string &app_name,
               bool use_debug_callback,
               std::vector<const char *> extension_names,
               std::vector<const char *> layer_names)
{
    if (use_debug_callback) {
        if (std::find(extension_names.begin(), extension_names.end(), VK_EXT_DEBUG_REPORT_EXTENSION_NAME)
            == extension_names.end()) {
            extension_names.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        if (std::find(layer_names.begin(), layer_names.end(), "VK_LAYER_LUNARG_standard_validation")
            == layer_names.end()) {
            layer_names.emplace_back("VK_LAYER_LUNARG_standard_validation");
        }
    }
    return std::make_shared<detail::GLCV>(app_name, extension_names, layer_names, use_debug_callback);
}

} // namespace glcv
