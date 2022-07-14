#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// struct Material
// {
uniform    sampler2D texture_diffuse_1, texture_diffuse_2, texture_diffuse_3;
uniform    sampler2D texture_specular_1, texture_specular_2;
// };
// uniform Material material;

void main()
{
    vec3 result = texture(texture_diffuse_1, TexCoords).rgb;
    // result += texture(texture_specular_1, TexCoords).rgb;
    FragColor = vec4(result, 1.0);
}