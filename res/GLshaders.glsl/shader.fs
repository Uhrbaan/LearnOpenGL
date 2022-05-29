#version 460 core
in vec3 base_color;
in vec2 texture_coo;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = vec4(base_color, 1.0)*texture(texture0, texture_coo);
}