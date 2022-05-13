#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoo;

out vec3 oCol;
out vec2 oTexCoo;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    oCol = aCol;
    oTexCoo = aTexCoo;
}