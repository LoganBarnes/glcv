// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCVForwardDeclarations.hpp>
#include <vulkan/vulkan.hpp>
#include <string>
#include <memory>
#include <vector>

namespace glcv {

namespace detail {

class GLCV
{
public:
    GLCV(const std::string &app_name,
         const std::vector<const char *> &extension_names,
         const std::vector<const char *> &layer_names,
         bool use_debug_callback);

    void set_surface(vk::SurfaceKHR surface);

    const vk::Instance &instance() const;
    const vk::SurfaceKHR &surface() const;
    const vk::PhysicalDevice &physical_device() const;
    const vk::Device &device() const;

private:
    std::shared_ptr<vk::Instance> instance_;
    std::shared_ptr<vk::SurfaceKHR> surface_;
    std::shared_ptr<vk::DebugReportCallbackEXT> debug_report_callback_;
    vk::PhysicalDevice physical_device_;
    std::shared_ptr<vk::Device> device_;
    vk::Queue graphics_queue_; // make this a map of queues eventually

    void init_instance(const std::string &app_name,
                       const std::vector<const char *> &extension_names,
                       const std::vector<const char *> &layer_names);

    void init_debug_report_callback();
    void init_physical_device();
    void init_device(const std::vector<const char *> &layer_names);
};

} // namespace detail

GLCV make_glcv(const std::string &app_name = "GLCV Application",
               bool use_debug_callback = false,
               std::vector<const char *> extension_names = {},
               std::vector<const char *> layer_names = {});

} // glcv
