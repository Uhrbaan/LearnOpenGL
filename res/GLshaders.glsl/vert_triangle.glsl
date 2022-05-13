#version 460 core
layout (location = 0) in vec3 aPos; // position has attribute location 0

void main()
{
    gl_Position = vec4(aPos, 1.0);
}