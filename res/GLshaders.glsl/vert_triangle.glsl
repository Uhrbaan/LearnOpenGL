#version 460 core
layout (location = 0) in vec3 aPos; // position has attribute location 0
layout (location = 1) in vec3 aColor; // color has attribute location 1

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // sets frag color from vertex data  
}