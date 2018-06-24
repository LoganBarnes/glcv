// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <glcv/detail/VulkanExt.hpp>
#include <glcv/GLCV.hpp>
#include "loop/SimpleLoop.hpp"
#include <glcv/VulkanUtil.hpp>
#include <glcv/util/vector_util.hpp>

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

        glcv_ = glcv::make_glcv("Example", debug, get_required_extensions());

#ifdef VERBOSE
        vk::PhysicalDeviceProperties device_props;
        util::print_vector("\nAvailable devices:", glcv_->instance().enumeratePhysicalDevices(), [&](auto &device) {
            device.getProperties(&device_props);
            return device_props.deviceName;
        });
#endif
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
    glcv::GLCV glcv_;
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
