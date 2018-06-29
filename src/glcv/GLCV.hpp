// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCVForwardDeclarations.hpp>
#include <glcv/ErrorCheck.hpp>
#include <string>
#include <memory>
#include <vector>

namespace glcv {

using SurfaceCreationFunction = std::add_pointer<vk::Result(VkInstance, VkSurfaceKHR *)>::type;

namespace detail {

class GLCV
{
public:
    template <typename SurfaceCreationFunctor>
    GLCV(const std::string &app_name,
         const std::vector<const char *> &extension_names,
         const std::vector<const char *> &layer_names,
         bool use_debug_callback,
         SurfaceCreationFunctor surface_functor)
    {
        init_instance(app_name, extension_names, layer_names);

        if (use_debug_callback) {
            init_debug_report_callback();
        }

        VkSurfaceKHR surface = nullptr;
        GLCV_CHECK(surface_functor(static_cast<VkInstance>(instance()), &surface));
        if (surface) {
            init_surface(surface);
        }

        init_physical_device();
        init_device(layer_names);
    }

    const vk::Instance &instance() const;
    const vk::SurfaceKHR &surface() const;
    const vk::PhysicalDevice &physical_device() const;
    const vk::Device &device() const;

private:
    std::shared_ptr<vk::Instance> instance_;
    std::shared_ptr<vk::DebugReportCallbackEXT> debug_report_callback_;
    std::shared_ptr<vk::SurfaceKHR> surface_;
    vk::PhysicalDevice physical_device_;
    std::shared_ptr<vk::Device> device_;
    vk::Queue graphics_queue_; // make this a map of queues eventually
    vk::Queue present_queue_; // make this a map of queues eventually

    void init_instance(const std::string &app_name,
                       const std::vector<const char *> &extension_names,
                       const std::vector<const char *> &layer_names);

    void init_debug_report_callback();
    void init_surface(vk::SurfaceKHR surface);
    void init_physical_device();
    void init_device(const std::vector<const char *> &layer_names);
};

} // namespace detail

struct EmptySurfaceFunctor
{
    VkResult operator()(VkInstance, VkSurfaceKHR *) { return VK_SUCCESS; }
};

template <typename SurfaceCreationFunctor = EmptySurfaceFunctor>
GLCV make_glcv(const std::string &app_name = "GLCV Application",
               bool use_debug_callback = false,
               std::vector<const char *> extension_names = {},
               std::vector<const char *> layer_names = {},
               SurfaceCreationFunctor surface_functor = SurfaceCreationFunctor{})
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
    return std::make_shared<detail::GLCV>(app_name, extension_names, layer_names, use_debug_callback, surface_functor);
}

} // glcv
