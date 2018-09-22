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

struct EmptySurfaceFunctor
{
    VkResult operator()(VkInstance, VkSurfaceKHR *) { return VK_SUCCESS; }
};

class GLCV
{
public:
    const vk::Instance &instance() const;
    const vk::SurfaceKHR &surface() const;
    const vk::PhysicalDevice &physical_device() const;
    const vk::Device &device() const;

    template <typename SurfaceCreationFunctor = EmptySurfaceFunctor>
    static std::shared_ptr<GLCV> create_shared_instance(const std::string &app_name = "GLCV Application",
                                                        bool use_debug_callback = false,
                                                        std::vector<const char *> extension_names = {},
                                                        std::vector<const char *> layer_names = {},
                                                        SurfaceCreationFunctor surface_functor
                                                        = SurfaceCreationFunctor{});

private:
    template <typename SurfaceCreationFunctor>
    explicit GLCV(const std::string &app_name,
                  const std::vector<const char *> &extension_names,
                  const std::vector<const char *> &layer_names,
                  bool use_debug_callback,
                  SurfaceCreationFunctor surface_functor)
        : graphics_family_{std::numeric_limits<uint32_t>::max()}
        , present_family_{std::numeric_limits<uint32_t>::max()}
        , surface_format_{vk::Format::eUndefined, vk::ColorSpaceKHR::eSrgbNonlinear}
    {
        init_instance(app_name, extension_names, layer_names);

        if (use_debug_callback) {
            init_debug_report_callback();
        }

        VkSurfaceKHR surface;
        uint32_t width, height;
        GLCV_CHECK(surface_functor(static_cast<VkInstance>(instance()), &surface, &width, &height));
        if (surface) {
            init_surface(vk::SurfaceKHR(surface));
        }

        init_physical_device();
        init_device(layer_names);

        if (surface) {
            init_swapchain(width, height);
            init_swapchain_images();
        }
    }

    std::shared_ptr<vk::Instance> instance_;
    std::shared_ptr<vk::DebugReportCallbackEXT> debug_report_callback_;
    std::shared_ptr<vk::SurfaceKHR> surface_;
    vk::PhysicalDevice physical_device_;
    std::shared_ptr<vk::Device> device_;
    uint32_t graphics_family_;
    uint32_t present_family_;
    vk::Queue graphics_queue_; // make this a map of queues eventually
    vk::Queue present_queue_; // make this a map of queues eventually

    std::shared_ptr<vk::SwapchainKHR> swapchain_;
    std::vector<vk::Image> swapchain_images_;
    vk::SurfaceFormatKHR surface_format_;
    vk::Extent2D extent_;

    std::vector<std::shared_ptr<vk::ImageView>> swapchain_image_views_;

    void init_instance(const std::string &app_name,
                       const std::vector<const char *> &extension_names,
                       const std::vector<const char *> &layer_names);

    void init_debug_report_callback();
    void init_surface(vk::SurfaceKHR surface);
    void init_physical_device();
    void init_device(const std::vector<const char *> &layer_names);
    void init_swapchain(uint32_t width, uint32_t height);
    void init_swapchain_images();
};

template <typename SurfaceCreationFunctor>
std::shared_ptr<GLCV> GLCV::create_shared_instance(const std::string &app_name,
                                                   bool use_debug_callback,
                                                   std::vector<const char *> extension_names,
                                                   std::vector<const char *> layer_names,
                                                   SurfaceCreationFunctor surface_functor)
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

    // Cannot use make_shared here becasue GLCV constructor is private
    return std::shared_ptr<GLCV>(new GLCV(app_name, extension_names, layer_names, use_debug_callback, surface_functor));
}

} // glcv
