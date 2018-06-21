// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include <glcv/GLCV.hpp>
#include "loop/SimpleLoop.hpp"
#include <iostream>

class WireCube : public examples::SimpleLoop
{
public:
    WireCube() : examples::SimpleLoop("Wire Cube Example")
    {
        std::vector<VkExtensionProperties> available_ext = GLCV::get_available_extensions();

        std::cout << "Available extensions:" << std::endl;
        for (const auto &extension : available_ext) {
            std::cout << '\t' << extension.extensionName << std::endl;
        }

        std::vector<const char *> required_ext = get_required_extensions();

        std::cout << "\nRequired extensions:" << std::endl;
        for (const auto &extension : required_ext) {
            std::cout << '\t' << extension << std::endl;
        }

        GLCV::init("Example", required_ext);
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
