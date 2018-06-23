// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <glcv/detail/VulkanExt.hpp>
#include <glcv/GLCV.hpp>
#include "loop/SimpleLoop.hpp"
#include <glcv/VulkanUtil.hpp>
#include <glcv/util/vector_util.hpp>

#define VERBOSE

class WireCube : public examples::SimpleLoop
{
public:
    WireCube() : examples::SimpleLoop("Wire Cube Example")
    {
        instance_init();
        device_init();
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
    glcv::GLCV glcv_;

    void instance_init()
    {
#ifndef NDEBUG
        bool debug = true;
#else
        bool debug = false;
#endif

#ifdef VERBOSE
        util::print_vector("Available extensions:", glcv::get_available_extensions(), [](auto &ext) {
            return ext.extensionName;
        });
        util::print_vector("Available layers:", glcv::get_available_layers(), [](auto &layer) {
            return layer.layerName;
        });
#endif

        glcv_ = glcv::make_glcv("Example", debug, get_required_extensions());
    }

    void device_init()
    {
        vk::PhysicalDeviceProperties device_props;
        util::print_vector("\nAvailable devices:", glcv_->get_available_devices(), [&](auto &device) {
            device.getProperties(&device_props);
            return device_props.deviceName;
        });
    }
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
