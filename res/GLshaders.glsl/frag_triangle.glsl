#version 460 core
out vec4 FragColor; // we only want to output values -> 'out' named FragColor

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // RGBA
}