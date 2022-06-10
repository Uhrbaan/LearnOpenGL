#version 460 core
out vec4 FragColor;
uniform vec3 light_color;

void main()
{
    FragColor = vec4(normalize(light_color), 1.0);
}