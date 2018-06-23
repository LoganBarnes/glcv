// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <memory>
#include <vector>

class GLCV;

namespace glcv {
namespace detail {

class VulkanHandle;

} // namespace detail

using VulkanHandle = std::shared_ptr<detail::VulkanHandle>;

} // namespace glcv

class GLCV
{
public:
    static glcv::VulkanHandle init(const std::string &app_name = "GLCV Application",
                                   const std::vector<const char *> &extension_names = {},
                                   const std::vector<const char *> &layer_names = {},
                                   bool set_debug_callback = false);

    static std::vector<vk::PhysicalDevice> get_available_devices();

    GLCV(const GLCV &) = delete;
    GLCV(GLCV &&) noexcept = delete;
    GLCV &operator=(const GLCV &) = delete;
    GLCV &operator=(GLCV &&) noexcept = delete;

private:
    GLCV();

    static GLCV &self();

    std::weak_ptr<glcv::detail::VulkanHandle> handle_;

    static vk::Instance vkInstance();

    std::shared_ptr<vk::Instance> make_shared_instance(const std::string &app_name,
                                                       const std::vector<const char *> &extension_names,
                                                       const std::vector<const char *> &layer_names);

    std::shared_ptr<vk::DebugReportCallbackEXT> make_shared_debug_report_callback();
};
