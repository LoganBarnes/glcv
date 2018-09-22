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

class WireCube : public examples::SimpleLoop
{
public:
    WireCube() : examples::SimpleLoop("Wire Cube Example")
    {
#ifdef VERBOSE
        util::print_vector("Available extensions:", vk::enumerateInstanceExtensionProperties(), [](auto &ext) {
            return ext.extensionName;
        });
        util::print_vector("Available layers:", vk::enumerateInstanceLayerProperties(), [](auto &layer) {
            return layer.layerName;
        });
#endif

        auto surf_func = [&](VkInstance instance, VkSurfaceKHR *surface, uint32_t *width, uint32_t *height) {
            int w, h;
            glfwGetFramebufferSize(get_window(), &w, &h);
            *width = static_cast<uint32_t>(w);
            *height = static_cast<uint32_t>(h);
            return glfwCreateWindowSurface(instance, get_window(), nullptr, surface);
        };
        glcv_ = glcv::GLCV::create_shared_instance("Example", debug, get_required_extensions(), {}, surf_func);

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
            glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
        }
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
    std::shared_ptr<glcv::GLCV> glcv_;
    unsigned counter_ = 10000000;
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
