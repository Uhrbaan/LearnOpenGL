#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, uv);
    if(texColor.a < 0.1) // if not discarded will repeat texture according to params
        discard;
    FragColor = texColor;
}