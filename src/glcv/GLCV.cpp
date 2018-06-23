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
class VulkanHandle
{
public:
    ~VulkanHandle()
    {
        if (debug_report_callback_ && *debug_report_callback_) {
            instance_->destroy(*debug_report_callback_);
        }
        instance_->destroy(nullptr);
    }

private:
    std::shared_ptr<vk::Instance> instance_;
    std::shared_ptr<vk::DebugReportCallbackEXT> debug_report_callback_;
    friend class ::GLCV;
};
} // namespace detail
} // namespace glcv

glcv::VulkanHandle GLCV::init(const std::string &app_name,
                              const std::vector<const char *> &extension_names,
                              const std::vector<const char *> &layer_names,
                              bool set_debug_callback)
{
    GLCV &self = GLCV::self();

    if (auto handle = self.handle_.lock()) {
        return handle;
    }

    auto handle = std::make_shared<glcv::detail::VulkanHandle>();
    handle->instance_ = self.make_shared_instance(app_name, extension_names, layer_names);

    self.handle_ = handle; // now GLCV::vkInstance() will work

    if (set_debug_callback) {
        handle->debug_report_callback_ = self.make_shared_debug_report_callback();
    }
    return handle;
}

std::vector<vk::PhysicalDevice> GLCV::get_available_devices()
{
    return glcv::get_available_devices(GLCV::vkInstance());
}

GLCV::GLCV() = default;

GLCV &GLCV::self()
{
    static GLCV glcv;
    return glcv;
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
    auto instance = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr));
    //    auto instance = std::shared_ptr<vk::Instance>(new vk::Instance(nullptr), [](auto p) {
    //        if (*p) {
    //            p->destroy(nullptr);
    //            DEBUG_PRINT("Vulkan instance destroyed");
    //        }
    //        delete p;
    //    });

    GLCV_CHECK(vk::createInstance(&instance_info, nullptr, instance.get()));
    DEBUG_PRINT("Vulkan instance created");
    return instance;
}

std::shared_ptr<vk::DebugReportCallbackEXT> GLCV::make_shared_debug_report_callback()
{
    vk::Instance instance = GLCV::vkInstance();
    GLCV_CHECK(vkExtInitInstance(instance));

    const auto debug_info = vk::DebugReportCallbackCreateInfoEXT()
                                .setPNext(nullptr)
                                .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning)
                                .setPfnCallback(debug_callback)
                                .setPUserData(nullptr);

    auto debug_report_callback = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr));
    //    auto debug_report_callback
    //        = std::shared_ptr<vk::DebugReportCallbackEXT>(new vk::DebugReportCallbackEXT(nullptr), [instance](auto p) {
    //              if (*p) {
    //                  instance.destroy(*p, nullptr);
    //                  DEBUG_PRINT("Vulkan debug report callback destroyed");
    //              }
    //              delete p;
    //          });

    GLCV_CHECK(instance.createDebugReportCallbackEXT(&debug_info, nullptr, debug_report_callback.get()));
    DEBUG_PRINT("Vulkan debug report callback created");
    return debug_report_callback;
}

vk::Instance GLCV::vkInstance()
{
    if (auto handle = GLCV::self().handle_.lock()) {
        return *handle->instance_;
    }
    return nullptr;
}

//} // namespace glcv
