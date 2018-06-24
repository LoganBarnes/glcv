// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "GLCV.hpp"
#include "ErrorCheck.hpp"
#include "glcv/util/vector_util.hpp"
#include "glcv/detail/VulkanExt.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

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
    init_instance(app_name, extension_names, layer_names);

    if (set_debug_callback) {
        init_debug_report_callback();
    }
    init_physical_device();
    init_device(layer_names);
}

const vk::Instance &GLCV::instance() const
{
    return *instance_;
}

const vk::PhysicalDevice &GLCV::physical_device() const
{
    return physical_device_;
}

const vk::Device &GLCV::device() const
{
    return *device_;
}

void GLCV::init_instance(const std::string &app_name,
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

    instance_ = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr), [](auto* p) {
        if (*p) {
            p->destroy(nullptr);
            DEBUG_PRINT("Vulkan instance destroyed");
        }
        delete p;
    });

    GLCV_CHECK(vk::createInstance(&instance_info, nullptr, instance_.get()));
    DEBUG_PRINT("Vulkan instance created");
}

void GLCV::init_debug_report_callback()
{
    vk::Instance instance = *instance_;
    GLCV_CHECK(vkExtInitInstance(instance));

    const auto debug_info = vk::DebugReportCallbackCreateInfoEXT()
                                .setPNext(nullptr)
                                .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning)
                                .setPfnCallback(debug_callback)
                                .setPUserData(nullptr);

    debug_report_callback_
        = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr), [instance](auto* p) {
              if (*p) {
                  instance.destroy(*p, nullptr);
                  DEBUG_PRINT("Vulkan debug report callback destroyed");
              }
              delete p;
          });

    GLCV_CHECK(instance.createDebugReportCallbackEXT(&debug_info, nullptr, debug_report_callback_.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
}

void GLCV::init_physical_device()
{
    std::vector<vk::PhysicalDevice> devices = instance_->enumeratePhysicalDevices();

    if (devices.empty()) {
        throw std::runtime_error("GLCV ERROR: No suitable GPU found!");
    }

    physical_device_ = devices.front();
}

void GLCV::init_device(const std::vector<const char *> &layer_names)
{
    std::vector<vk::QueueFamilyProperties> queue_props = physical_device_.getQueueFamilyProperties();

    float queue_priorities = 0.f;
    auto queue_info
        = vk::DeviceQueueCreateInfo().setPNext(nullptr).setQueueCount(0).setPQueuePriorities(&queue_priorities);

    for (unsigned i = 0; i < queue_props.size(); i++) {
        if (queue_props[i].queueCount > 0 && (queue_props[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
            queue_info.setQueueCount(1).setQueueFamilyIndex(i);
            break;
        }
    }

    if (queue_info.queueCount == 0) {
        throw std::runtime_error("GLCV ERROR: Failed to find graphics queue for device");
    }

    const auto device_info = vk::DeviceCreateInfo()
                                 .setPNext(nullptr)
                                 .setQueueCreateInfoCount(1)
                                 .setPQueueCreateInfos(&queue_info)
                                 .setEnabledExtensionCount(0)
                                 .setPpEnabledExtensionNames(nullptr)
                                 .setEnabledLayerCount(static_cast<uint32_t>(layer_names.size()))
                                 .setPpEnabledLayerNames(layer_names.data())
                                 .setPEnabledFeatures(nullptr);

    device_ = std::shared_ptr<vk::Device>(new vk::Device(nullptr), [](auto* p) {
        if (*p) {
            p->destroy();
            DEBUG_PRINT("Vulkan device destroyed");
        }
        delete p;
    });

    GLCV_CHECK(physical_device_.createDevice(&device_info, nullptr, device_.get()));
    DEBUG_PRINT("Vulkan device created");

    graphics_queue_ = device_->getQueue(queue_info.queueFamilyIndex, 0);
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
