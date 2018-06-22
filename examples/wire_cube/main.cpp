// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <glcv/GLCV.hpp>
#include "loop/SimpleLoop.hpp"
#include <glcv/VulkanUtil.hpp>
#include <glcv/util/vector_util.hpp>

#define VERBOSE

class WireCube : public examples::SimpleLoop
{
public:
    WireCube() : examples::SimpleLoop("Wire Cube Example") { instance_init(); }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
    void instance_init()
    {
        bool debug = false;
        std::vector<const char *> requested_ext = get_required_extensions();
        std::vector<const char *> requested_layers = {};
#ifndef NDEBUG
        requested_ext.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        requested_layers.emplace_back("VK_LAYER_LUNARG_standard_validation");
        debug = true;
#endif

#ifdef VERBOSE
        util::print_vector("Available extensions:", glcv::get_available_extensions(), [](auto &ext) {
            return ext.extensionName;
        });
        util::print_vector("Requested extensions:", requested_ext);
        util::print_vector("Available layers:", glcv::get_available_layers(), [](auto &layer) {
            return layer.layerName;
        });
        util::print_vector("Requested layers:", requested_layers);
#endif

        GLCV::init("Example", requested_ext, requested_layers, debug);
    }
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
