#version 460 core
out vec4 FragColor;

in vec3 oCol;
in vec2 oTexCoo;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, oTexCoo);
    // builtin texture() fn takes a sampler and a texture coordinate
}