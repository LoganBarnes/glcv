// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <glcv/GLCV.hpp>
#include "loop/SimpleLoop.hpp"
#include "loop/ExampleConfig.hpp"
#include <iostream>

class WireCube : public examples::SimpleLoop
{
public:
    WireCube() : examples::SimpleLoop("Wire Cube Example")
    {
        std::vector<VkExtensionProperties> extensions = GLCV::get_available_extensions();

        std::cout << "Available extensions:" << std::endl;
        for (const auto &extension : extensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
        }
        GLCV::init("Example");
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
};

int main()
{
    WireCube example;
    example.run_loop();
    return 0;
}
