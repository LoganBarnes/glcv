// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "loop/SimpleLoop.hpp"
#include "glcv/GLCV.hpp"
#include "glcv/util/vector_util.hpp"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifndef NDEBUG
constexpr bool debug = true;
#else
constexpr bool debug = false;
#endif

#define VERBOSE

class VulkanOptions : public examples::SimpleLoop
{
public:
    VulkanOptions() : examples::SimpleLoop("Wire Cube Example", 640, 480, true, true)
    {
#ifdef VERBOSE
        util::print_vector("Available extensions:", vk::enumerateInstanceExtensionProperties(), [](auto &ext) {
            return ext.extensionName;
        });
        util::print_vector("Available layers:", vk::enumerateInstanceLayerProperties(), [](auto &layer) {
            return layer.layerName;
        });
#endif

        auto surf_func = [&](VkInstance instance, VkSurfaceKHR *surface) {
            return glfwCreateWindowSurface(instance, get_vulkan_window(), nullptr, surface);
        };
        glcv_ = glcv::make_glcv("Example", debug, get_required_extensions(), {}, surf_func);
        // glcv_ = glcv::make_glcv("Example", debug, get_required_extensions());

#ifdef VERBOSE
        vk::PhysicalDeviceProperties device_props;
        util::print_vector("\nAvailable devices:", glcv_->instance().enumeratePhysicalDevices(), [&](auto &device) {
            device.getProperties(&device_props);
            return device_props.deviceName;
        });
#endif
    }

    void update(float /*sim_time*/, float /*time_step*/) final
    {
        if (!glcv_->surface()) {
            --counter_;
        }

        if (counter_ == 0) {
            glfwSetWindowShouldClose(get_vulkan_window(), GLFW_TRUE);
        }
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
    glcv::GLCV glcv_;
    unsigned counter_ = 10000000;
};

int main()
{
    VulkanOptions example;
    example.run_loop();
    return 0;
}
