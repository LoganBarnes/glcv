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
#include <unordered_set>

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

const vk::Instance &GLCV::instance() const
{
    return *instance_;
}

const vk::SurfaceKHR &GLCV::surface() const
{
    return *surface_;
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

    instance_ = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr), [](auto *p) {
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
        = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr), [instance](auto *p) {
              if (*p) {
                  instance.destroy(*p, nullptr);
                  DEBUG_PRINT("Vulkan debug report callback destroyed");
              }
              delete p;
          });

    GLCV_CHECK(instance.createDebugReportCallbackEXT(&debug_info, nullptr, debug_report_callback_.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
}

void GLCV::init_surface(vk::SurfaceKHR surface)
{
    surface_ = nullptr;
    surface_ = std::shared_ptr<vk::SurfaceKHR>(new vk::SurfaceKHR(surface), [this](auto *p) {
        if (*p) {
            instance_->destroy(*p);
            DEBUG_PRINT("Surface destroyed");
        }
        delete p;
    });
    DEBUG_PRINT("Surface set");
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

    bool graphics_set = false;
    bool present_set = false;
    uint32_t graphics_family = std::numeric_limits<uint32_t>::max();
    uint32_t present_family = std::numeric_limits<uint32_t>::max();

    std::unordered_set<uint32_t> unique_queue_families;

    for (uint32_t i = 0; i < queue_props.size(); i++) {
        if (!graphics_set && (queue_props[i].queueCount > 0)
            && (queue_props[i].queueFlags & vk::QueueFlagBits::eGraphics)) {
            graphics_family = i;
            graphics_set = true;
            unique_queue_families.emplace(i);
        }

        if (surface_) {
            vk::Bool32 present_support;
            physical_device_.getSurfaceSupportKHR(i, *surface_, &present_support);

            if (!present_set && (queue_props[i].queueCount > 0 && present_support)) {
                present_family = i;
                present_set = true;
                unique_queue_families.emplace(i);
            }
        }

        if (graphics_set && present_set) {
            break;
        }
    }

    if (!graphics_set) {
        throw std::runtime_error("GLCV ERROR: Failed to find device capable of graphics");
    }
    if (surface_ && !present_set) {
        throw std::runtime_error("GLCV ERROR: Failed to find device capable of presentation");
    }

    std::vector<vk::DeviceQueueCreateInfo> queue_infos;

    float queue_priority = 0.f;
    for (uint32_t queue_family : unique_queue_families) {
        queue_infos.emplace_back(vk::DeviceQueueCreateInfo()
                                     .setPNext(nullptr)
                                     .setQueueFamilyIndex(queue_family)
                                     .setQueueCount(1)
                                     .setPQueuePriorities(&queue_priority));
    }

    const auto device_info = vk::DeviceCreateInfo()
                                 .setPNext(nullptr)
                                 .setQueueCreateInfoCount(static_cast<uint32_t>(queue_infos.size()))
                                 .setPQueueCreateInfos(queue_infos.data())
                                 .setEnabledExtensionCount(0)
                                 .setPpEnabledExtensionNames(nullptr)
                                 .setEnabledLayerCount(static_cast<uint32_t>(layer_names.size()))
                                 .setPpEnabledLayerNames(layer_names.data())
                                 .setPEnabledFeatures(nullptr);

    device_ = std::shared_ptr<vk::Device>(new vk::Device(nullptr), [](auto *p) {
        if (*p) {
            p->destroy();
            DEBUG_PRINT("Vulkan device destroyed");
        }
        delete p;
    });

    GLCV_CHECK(physical_device_.createDevice(&device_info, nullptr, device_.get()));
    DEBUG_PRINT("Vulkan device created");

    graphics_queue_ = device_->getQueue(graphics_family, 0);

    if (surface_) {
        present_queue_ = device_->getQueue(present_family, 0);
    }
}

} // namespace detail

} // namespace glcv
