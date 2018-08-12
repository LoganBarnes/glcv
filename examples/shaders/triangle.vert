// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(location = 0) out vec3 frag_color;

vec2 positions[3] = vec2[]
(
    vec2( 0.0f, -0.5f),
    vec2( 0.5f,  0.5f),
    vec2(-0.5f,  0.5f)
);

vec3 colors[3] = vec3[]
(
    vec3(1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, 0.f, 1.f)
);

void main(void)
{
    gl_Position = vec4(positions[gl_VertexIndex], 0.f, 1.f);
    frag_color = colors[gl_VertexIndex];
}
