#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

// struct Material
// {
uniform    sampler2D texture_diffuse[3];
uniform    sampler2D texture_specular[3];
// };
// uniform Material material;

void main()
{
    vec3 result = texture(texture_diffuse[0], TexCoords).rgb;
    // result *= texture(texture_specular[0], TexCoords).rgb;
    FragColor = vec4(result, 1.0);
}