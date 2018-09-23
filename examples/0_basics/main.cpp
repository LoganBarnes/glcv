// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "loop/SimpleLoop.hpp"
#include "glcv/GLCV.hpp"
#include "glcv/GraphicsPipeline.hpp"
#include "glcv/util/vector_util.hpp"
#include "examples/ExampleConfig.hpp"

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
        int w, h;
        glfwGetFramebufferSize(get_window(), &w, &h);
        auto uw = static_cast<uint32_t>(w);
        auto uh = static_cast<uint32_t>(h);

        auto surf_func = [&](VkInstance instance, VkSurfaceKHR *surface, uint32_t *width, uint32_t *height) {
            *width = uw;
            *height = uh;
            return glfwCreateWindowSurface(instance, get_window(), nullptr, surface);
        };
        glcv_ = glcv::GLCV::create_shared_instance("Example", debug, get_required_extensions(), {}, surf_func);

        std::vector<std::pair<std::string, vk::ShaderStageFlagBits>> spirv_files = {
            {examples::spirv_path() + "triangle_vert.spv", vk::ShaderStageFlagBits::eVertex},
            {examples::spirv_path() + "triangle_frag.spv", vk::ShaderStageFlagBits::eFragment},
        };

        auto pipeline = glcv::GraphicsPipeline(glcv_, spirv_files, uw, uh);
        (void)pipeline; //  until we use this

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
    unsigned counter_ = 10000000;
    std::shared_ptr<glcv::GLCV> glcv_;
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
