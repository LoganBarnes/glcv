// ////////////////////////////////////////////////////////////
// Graphics Library Classes
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "SimpleLoop.hpp"

#include <vgc/VGC.hpp>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw_vulkan.h>

namespace examples {

namespace {

} // namespace

SimpleLoop::SimpleLoop(const std::string &title, int width, int height, bool resizable)
{
    // Set the error callback before any other GLFW calls so we get proper error reporting
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });

    init_glfw();
    create_window(title, width, height, resizable);
    set_callbacks();
    resize(width, height);
}

SimpleLoop::~SimpleLoop() = default;

void SimpleLoop::run_loop()
{
    auto currentTime = std::chrono::steady_clock::now();
    double accumulator = 0.0;

    do {
        auto newTime = std::chrono::steady_clock::now();
        double frameTime = std::chrono::duration<double>{newTime - currentTime}.count();
        currentTime = newTime;

        frameTime = std::min(0.1, frameTime);

        if (!paused_) {
            accumulator += frameTime;

            while (accumulator >= time_step_) {
                update(static_cast<float>(sim_time_), static_cast<float>(time_step_));
                sim_time_ += time_step_;
                accumulator -= time_step_;
            }
        }

        const double alpha = accumulator / time_step_;

        int w, h;
        glfwGetFramebufferSize(window_.get(), &w, &h);
        render(w, h, static_cast<float>(alpha));

        //        glfwSwapBuffers(window_.get());

        if (paused_) {
            glfwWaitEvents();
        } else {
            glfwPollEvents();
        }
    } while (!glfwWindowShouldClose(window_.get()));
}

int SimpleLoop::get_framebuffer_width() const
{
    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);
    return w;
}

int SimpleLoop::get_framebuffer_height() const
{
    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);
    return h;
}

void SimpleLoop::init_glfw()
{
    glfw_ = std::shared_ptr<int>(new int(glfwInit()), [](auto p) {
        glfwTerminate();
        delete p;
    });

    if (*glfw_ == 0) {
        throw std::runtime_error("GLFW init failed");
    }
}

void SimpleLoop::create_window(const std::string &title, int width, int height, bool resizable)
{
    //    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // until I learn how to do this in vulkan

    // no api needed for Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window_ = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), [](auto p) {
        if (p) {
            glfwDestroyWindow(p);
        }
    });

    if (window_ == nullptr) {
        throw std::runtime_error("GLFW window creation failed");
    }

    //    ImGui::CreateContext();
    //    ImGuiIO &io = ImGui::GetIO();
    //    (void)io;
    //    ImGui_ImplGlfwVulkan_Init_Data init_data = {};
    //    init_data.allocator = g_Allocator;
    //    init_data.gpu = g_Gpu;
    //    init_data.device = g_Device;
    //    init_data.render_pass = g_RenderPass;
    //    init_data.pipeline_cache = g_PipelineCache;
    //    init_data.descriptor_pool = g_DescriptorPool;
    //    init_data.check_vk_result = check_vk_result;

    //    imgui_ = std::shared_ptr<bool>(new bool(ImGui_ImplGlfwVulkan_Init(window_.get(), true, &init_data)), [](auto p) {
    //        ImGui_ImplGlfwVulkan_Shutdown();
    //        ImGui::DestroyContext();
    //        delete p;
    //    });
}

void SimpleLoop::set_callbacks()
{
    glfwSetWindowUserPointer(window_.get(), this);

    glfwSetFramebufferSizeCallback(window_.get(), [](GLFWwindow *window, int width, int height) {
        static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window))->resize(width, height);
    });

    glfwSetMouseButtonCallback(window_.get(), [](GLFWwindow *window, int button, int action, int) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                simple_loop->left_mouse_down_ = true;
                glfwGetCursorPos(window, &simple_loop->prev_mouseX_, &simple_loop->prev_mouseY_);
            } else if (action == GLFW_RELEASE) {
                simple_loop->left_mouse_down_ = false;
            }
        }
    });

    glfwSetKeyCallback(window_.get(), [](GLFWwindow *window, int key, int, int action, int) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
            simple_loop->paused_ = !simple_loop->paused_;
        }
    });

    glfwSetCursorPosCallback(window_.get(), [](GLFWwindow *window, double xpos, double ypos) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (simple_loop->left_mouse_down_) {
            simple_loop->handle_mouse_drag(static_cast<float>(xpos - simple_loop->prev_mouseX_),
                                           static_cast<float>(simple_loop->prev_mouseY_ - ypos));
            simple_loop->prev_mouseX_ = xpos;
            simple_loop->prev_mouseY_ = ypos;
        }
    });

    glfwSetScrollCallback(window_.get(), [](GLFWwindow *window, double, double yoffset) {
        static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window))->handle_scroll(static_cast<float>(yoffset));
    });
}

} // namespace examples
