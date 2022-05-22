#version 460 core
out vec4 FragColor;
in vec3 base_color;

uniform sampler2D utexture;

void main()
{
    FragColor = vec4(base_color, 1.0) + texture(utexture, texture_coo);
}