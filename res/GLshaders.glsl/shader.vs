#version 460 core
layout (location = 0) in vec3 v_vertices_pos;
layout (location = 1) in vec3 v_base_color;
layout (location = 2) in vec2 v_texture_coo;

out vec3 base_color;
out vec2 texture_coo;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(v_vertices_pos, 1.0);
    base_color = v_base_color;
    texture_coo = v_texture_coo;
}