#version 460 core
out vec4 FragColor; // 'out' named FragColor
in vec4 vertex_color; // input from vert_triangle.glsl->vertex_color

uniform vec4 add_color; // global value to openGL

void main()
{
    FragColor = vertex_color + add_color;
}