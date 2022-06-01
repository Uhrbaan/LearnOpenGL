#version 460 core
in vec3 o_col;
in vec2 o_tex;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = vec4(o_col, 1.0)*texture(texture0, o_tex);
}