#version 460 core
layout (location = 0) in vec3 aPos; // attribute position 0
// let the vertex shader decide the color of the fragment shader
out vec4 vertex_color; // color output the passed to fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // give a vec3 to vec 4 for position
    vertex_color = vec4(0.6, 0.0, 0.0, 1.0);
}