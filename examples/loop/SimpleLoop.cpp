// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "SimpleLoop.hpp"
#include "glcv/GLCV.hpp"

#include "glcv/GLCV.hpp"
#include "glcv/ErrorCheck.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <algorithm>
#include <imgui.h>

#include <imgui_impl_glfw_gl3.h>

namespace examples {

namespace {
std::shared_ptr<GLFWwindow> create_window(const std::string &title, int width, int height, bool resizable)
{
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    auto window
        = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), [](auto p) {
              if (p) {
                  glfwDestroyWindow(p);
              }
          });

    if (window == nullptr) {
        throw std::runtime_error("GLFW window creation failed");
    }

    return window;
}
} // namespace

SimpleLoop::SimpleLoop(const std::string &title, int width, int height, bool resizable, bool create_opengl_gui_window)
{
    // Set the error callback before any other GLFW calls so we get proper error reporting
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });

    init_glfw();
    init_imgui();
    create_vulkan_window(title, width, height);

    if (create_opengl_gui_window) {
        create_opengl_window(title + " GUI", width, height, resizable);
    }

    resize(width, height);
}

SimpleLoop::~SimpleLoop() = default;

std::vector<const char *> SimpleLoop::get_required_extensions() const
{
    uint32_t count;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&count);

    std::vector<const char *> extensions;
    for (uint32_t i = 0; i < count; ++i) {
        extensions.emplace_back(glfw_extensions[i]);
    }
    return extensions;
}

GLFWwindow *SimpleLoop::get_vulkan_window() const
{
    return vulkan_window_.get();
}

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
        glfwGetFramebufferSize(get_vulkan_window(), &w, &h);

        setup_and_render(w, h, static_cast<float>(alpha));

        if (opengl_window_) {
            glfwSwapBuffers(opengl_window_.get());
        }

        if (paused_) {
            glfwWaitEvents();
        } else {
            glfwPollEvents();
        }
    } while (!glfwWindowShouldClose(get_vulkan_window()));
}

void SimpleLoop::init_glfw()
{
    glfw_ = std::shared_ptr<int>(new int(glfwInit()), [](auto *p) {
        glfwTerminate();
        delete p;
    });

    if (*glfw_ == 0) {
        throw std::runtime_error("GLFW init failed");
    }
}

void SimpleLoop::init_imgui()
{
    imgui_ = std::shared_ptr<ImGuiContext>(ImGui::CreateContext(), [](auto p) { ImGui::DestroyContext(p); });

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    if (imgui_ == nullptr) {
        throw std::runtime_error("ImGui init failed");
    }
}

void SimpleLoop::create_vulkan_window(const std::string &title, int width, int height)
{
    // no api needed for Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // no resize until I learn how to do this in vulkan
    vulkan_window_ = create_window(title, width, height, false);

    set_callbacks(vulkan_window_.get());
}

void SimpleLoop::create_opengl_window(const std::string &title, int width, int height, bool resizable)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    opengl_window_ = create_window(title, width, height, resizable);

    glfwMakeContextCurrent(opengl_window_.get());
    glfwSwapInterval(1);

    if (gl3wInit()) {
        throw std::runtime_error("Failed to initialize OpenGL");
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    opengl_imgui_ = std::shared_ptr<bool>( //
        new bool(ImGui_ImplGlfwGL3_Init(opengl_window_.get(), false)),
        [](auto p) {
            ImGui_ImplGlfwGL3_Shutdown();
            delete p;
        });

    ImGui::StyleColorsDark();

    set_callbacks(opengl_window_.get());
}

void SimpleLoop::set_callbacks(GLFWwindow *glfw_window)
{
    glfwSetWindowUserPointer(glfw_window, this);

    glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow *window, int width, int height) {
        static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window))->resize(width, height);
    });

    glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow *window, int button, int action, int mods) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                simple_loop->left_mouse_down_ = true;
                glfwGetCursorPos(window, &simple_loop->prev_mouseX_, &simple_loop->prev_mouseY_);
            } else if (action == GLFW_RELEASE) {
                simple_loop->left_mouse_down_ = false;
            }
        }

        if (simple_loop->get_vulkan_window() == window) {
            // do vulkan gui stuff?
        } else {
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        }
    });

    glfwSetKeyCallback(glfw_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
            simple_loop->paused_ = !simple_loop->paused_;
        }

        if (simple_loop->get_vulkan_window() == window) {
            // do vulkan gui stuff?
        } else {
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        }
    });

    glfwSetCursorPosCallback(glfw_window, [](GLFWwindow *window, double xpos, double ypos) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));
        if (simple_loop->left_mouse_down_) {
            simple_loop->handle_mouse_drag(static_cast<float>(xpos - simple_loop->prev_mouseX_),
                                           static_cast<float>(simple_loop->prev_mouseY_ - ypos));
            simple_loop->prev_mouseX_ = xpos;
            simple_loop->prev_mouseY_ = ypos;
        }
    });

    glfwSetScrollCallback(glfw_window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));

        simple_loop->handle_scroll(static_cast<float>(yoffset));

        if (simple_loop->get_vulkan_window() == window) {
            // do vulkan gui stuff?
        } else {
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        }
    });

    glfwSetCharCallback(glfw_window, [](GLFWwindow *window, unsigned c) {
        auto simple_loop = static_cast<SimpleLoop *>(glfwGetWindowUserPointer(window));

        if (simple_loop->get_vulkan_window() == window) {
            // do vulkan gui stuff?
        } else {
            ImGui_ImplGlfw_CharCallback(window, c);
        }
    });
}
void SimpleLoop::setup_and_render(int w, int h, float alpha)
{
    if (opengl_window_) {
        int glw, glh;
        glfwGetFramebufferSize(opengl_window_.get(), &glw, &glh);

        glViewport(0, 0, glw, glh);

        if (paused_) {
            ImGui_ImplGlfwGL3_NewFrame();
            render_gui(glw, glh);
            ImGui::Render();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplGlfwGL3_NewFrame();
        render_gui(glw, glh);

        ImGui::Render();
    }

    render(w, h, alpha);
}

} // namespace examples
