// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "loop/SimpleLoop.hpp"
#include <iostream>

class WireCube2 : public examples::SimpleLoop
{
public:
    WireCube2() : examples::SimpleLoop("Wire Cube Example", 640, 480, true) {}

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

private:
};

int main()
{
    WireCube2 example;
    example.run_loop();
    return 0;
}
