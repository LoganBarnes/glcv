// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "SimpleLoop.hpp"

#include <glcv/GLCV.hpp>

#include <iostream>

namespace examples {

namespace {

} // namespace

SimpleLoop::SimpleLoop(const std::string &, int, int, bool) {}

std::vector<const char *> SimpleLoop::get_required_extensions() const
{
    return {};
}

SimpleLoop::~SimpleLoop() = default;

void SimpleLoop::run_loop()
{
    for (int i = 0; i < 1000; ++i) {
        update(static_cast<float>(sim_time_), static_cast<float>(time_step_));
        sim_time_ += time_step_;
        render(640, 480, 1.f);
    }
}

void SimpleLoop::init_glfw() {}

void SimpleLoop::create_window(const std::string &title, int width, int height, bool resizable) {}

void SimpleLoop::set_callbacks() {}

} // namespace examples
