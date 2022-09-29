#version 460 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 iuv;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = iuv;
    gl_Position = /* projection * view * model * */vec4(vert_pos, 1.0);
}