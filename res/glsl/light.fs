#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec3 Normal;

struct Material
{
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;
struct Light
{
    vec3 position;
    
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform vec3 cam_pos;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_dir = normalize(cam_pos-frag_pos);
    
    // ambient
    vec3 ambient = light.ambiant * material.ambiant;
    
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}