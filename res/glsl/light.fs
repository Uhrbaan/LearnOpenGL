#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec3 Normal;
in vec2 texture_coo;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;
struct Light
{
    vec3 position, direction;
    
    vec3 ambient, diffuse, specular;

    // spotlight
    float cutoff, outer_cutoff;

    // attenuation over distance
    float constant, linear, quadratic;
};
uniform Light light;
uniform vec3 cam_pos;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_dir = normalize(cam_pos-frag_pos);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, texture_coo).rgb;
    
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * 
                vec3(texture(material.diffuse, texture_coo));
    
    // specular
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * 
                    texture(material.specular, texture_coo).rgb;

    // spotlight (soft edges) | to make spotlight smooth -> I=(𝜃-𝛾)/𝜀
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta-light.outer_cutoff)/epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular*= intensity;

    // attenuation -> F=1/(Kc + Kl·d + Kq·d²)
    float d = length(light.position-frag_pos);
    float attenuation = 
        1.0/(light.constant + light.linear*d + light.quadratic*(d*d));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;
    
    vec3 result = (ambient + diffuse + specular)+ texture(material.emission, texture_coo).rgb/2;
    FragColor = vec4(result, 1.0);
}