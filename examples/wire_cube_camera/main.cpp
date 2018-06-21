// ////////////////////////////////////////////////////////////
// Graphics Library Classes
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "loop/SimpleLoop.hpp"
#include "loop/ExampleConfig.hpp"
#include <glc/Program.hpp>
#include <glc/VertexArray.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class WireCube2 : public examples::SimpleLoop
{
public:
    WireCube2() : examples::SimpleLoop("Wire Cube Example", 640, 480, true), camera_mover_(glc::Camera())
    {
        glc::setCommonDefaults();

        // The 8 corners of the cube
        std::vector<glm::vec3> vertices{
            // front face (pos-Z)
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},
            // back face (neg-Z)
            {-1, 1, -1},
            {1, 1, -1},
            {1, -1, -1},
            {-1, -1, -1},
        };

        // Optimized 14 point single triangle strip
        std::vector<unsigned> indices = {3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0};

        // OpenGL setup to render this cube
        program_
            = glc::create_program(examples::shader_path() + "default.vert", examples::shader_path() + "color.frag");
        vbo_ = glc::create_buffer(vertices);
        ibo_ = glc::create_buffer<GL_ELEMENT_ARRAY_BUFFER>(indices);
        vao_ = glc::create_vertex_array(program_, vbo_, 0, {{"local_position", 3, GL_FLOAT, nullptr}});

        camera_mover_.camera.setAspectRatio(get_framebuffer_width() * 1.f / get_framebuffer_height());
        camera_mover_.setOrbitOffsetDistance(5.f);
    }

    void render(int view_width, int view_height, float) const final
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, view_width, view_height);

        program_->use([&] {
            program_->set_matrix_uniform("screen_from_world",
                                         glm::value_ptr(camera_mover_.camera.getPerspectiveScreenFromWorldMatrix()));

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            vao_->render(GL_TRIANGLE_STRIP, 0, 14, ibo_);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        });
    }

    void handle_mouse_drag(float deltaX, float deltaY) final
    {
        float scale_factor = 0.5f;
        camera_mover_.yaw(-deltaX * scale_factor);
        camera_mover_.pitch(deltaY * scale_factor);
    }

    void handle_scroll(float delta) final
    {
        float scale_factor = -2.5f;
        camera_mover_.zoom(delta * scale_factor);
    }

    void resize(int view_width, int view_height) final
    {
        camera_mover_.camera.setAspectRatio(view_width * 1.f / view_height);
    }

private:
    glc::Program program_;
    glc::Buffer<glm::vec3> vbo_;
    glc::Buffer<unsigned> ibo_;
    glc::VertexArray vao_;

    glc::CameraMover camera_mover_;
};

int main()
{
    WireCube2 example;
    example.run_loop();
    return 0;
}
