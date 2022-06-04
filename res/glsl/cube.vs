#version 460 core
layout (location = 0) in vec3 i_pos;

uniform mat4 u_transform;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view  * u_transform * vec4(i_pos, 1.0);
}

// for both fs shaders