#version 460 core
out vec4 FragColor;
in vec3 base_color;
in vec2 texture_coo;

uniform sampler2D sampler_2d_texture;

void main()
{
    FragColor = texture(sampler_2d_texture, texture_coo);
}