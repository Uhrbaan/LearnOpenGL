#version 460 core
layout (location = 0) in vec3 v_vertices_pos;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture_coo;

out vec3 base_color;
out vec2 texture_coo;

uniform mat4 model, view, projection;

void main()
{
    gl_Position = /* projection * view * model *  */vec4(v_vertices_pos, 1.0);
    texture_coo = v_texture_coo;
    base_color  = v_color;
}