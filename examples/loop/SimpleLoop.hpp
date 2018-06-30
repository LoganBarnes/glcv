// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCVForwardDeclarations.hpp>
#include <functional>
#include <string>
#include <vector>

struct GLFWwindow;
struct ImGuiContext;

namespace examples {

class SimpleLoop
{
public:
    explicit SimpleLoop(const std::string &title = "Example",
                        int width = 640,
                        int height = 480,
                        bool resizable = false,
                        bool create_opengl_gui_window = false);
    virtual ~SimpleLoop();

    virtual void update(float /*sim_time*/, float /*time_step*/) {}
    virtual void render(int /*view_width*/, int /*view_height*/, float /*alpha*/) const {}

    virtual void render_gui(int /*view_width*/, int /*view_height*/) const {};

    virtual void handle_mouse_drag(float /*deltaX*/, float /*deltaY*/) {}
    virtual void handle_scroll(float /*delta*/) {}

    virtual void resize(int /*view_width*/, int /*view_height*/) {}

    void run_loop();

    std::vector<const char *> get_required_extensions() const;
    GLFWwindow *get_vulkan_window() const;

protected:
    double time_step_ = 1.0 / 60.0; // 60 fps
    double sim_time_ = 0.0;
    bool paused_ = true;

private:
    bool left_mouse_down_ = false;
    bool padding_[6];

    double prev_mouseX_ = 0.0;
    double prev_mouseY_ = 0.0;

    std::shared_ptr<int> glfw_;
    std::shared_ptr<ImGuiContext> imgui_;

    std::shared_ptr<GLFWwindow> vulkan_window_;
    std::shared_ptr<bool> vulkan_imgui_;

    std::shared_ptr<GLFWwindow> opengl_window_;
    std::shared_ptr<bool> opengl_imgui_;

    void init_glfw();
    void init_imgui();
    void create_vulkan_window(const std::string &title, int width, int height);
    void create_opengl_window(const std::string &title, int width, int height, bool resizable);
    void set_callbacks(GLFWwindow *glfw_window);

    void setup_and_render(int w, int h, float alpha);
};

} // namespace examples
