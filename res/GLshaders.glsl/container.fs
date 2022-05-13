#version 460 core
out vec4 FragColor;

in vec3 oCol;
in vec2 oTexCoo;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 brightness;

void main()
{
    FragColor = mix(texture(texture1, oTexCoo),
                    texture(texture2, oTexCoo),
                    0.1)
                * vec4(oCol, 1.0) 
                + vec4(brightness, 1.0);
    // builtin texture() fn takes a sampler and a texture coordinate
}