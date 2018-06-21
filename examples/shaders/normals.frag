// ////////////////////////////////////////////////////////////
// Graphics Library Classes
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in Vertex
{
    vec3 world_position;
    vec3 world_normal;
    vec2 tex_coords;
    vec3 color;
} vertex;

layout(location = 0) out vec4 out_color;

void main(void)
{
    vec3 normal = gl_FrontFacing ? vertex.world_normal : -vertex.world_normal;
    normal = normal * 0.5 + 0.5;
    out_color = vec4(normal, 1.0);
}
