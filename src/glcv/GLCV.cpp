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

std::vector<vk::PhysicalDevice> GLCV::get_available_devices()
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

    instance_ = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr), [](auto p) {
        if (*p) {
            p->destroy(nullptr);
            DEBUG_PRINT("Vulkan instance destroyed");
        }
        delete p;
    });

    GLCV_CHECK(vk::createInstance(&instance_info, nullptr, instance_.get()));
    DEBUG_PRINT("Vulkan instance created");
}

void GLCV::setup_debug_callback()
{
    GLCV_CHECK(vkExtInitInstance(*instance_));

    const auto debug_info = vk::DebugReportCallbackCreateInfoEXT()
                                .setPNext(nullptr)
                                .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning)
                                .setPfnCallback(debug_callback)
                                .setPUserData(nullptr);

    auto instance = instance_;
    debug_callback_
        = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr), [instance](auto p) {
              if (*p) {
                  instance->destroy(*p, nullptr);
                  DEBUG_PRINT("Vulkan debug report callback destroyed");
              }
              delete p;
          });

    GLCV_CHECK(instance_->createDebugReportCallbackEXT(&debug_info, nullptr, debug_callback_.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
}

void GLCV::destroy()
{
    GLCV::self().debug_callback_ = nullptr;
    GLCV::self().instance_ = nullptr;
}

//} // namespace glcv
