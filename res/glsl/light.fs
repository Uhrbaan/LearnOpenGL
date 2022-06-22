#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec3 Normal;
in vec2 texture_coo;

struct Material
{
    sampler2D diffuse, specular; // temporary to remove errors
    sampler2D texture_diffuse_1, texture_diffuse_2, texture_diffuse_3;
    sampler2D texture_specular_1, texture_specular_2;
    sampler2D emission;
    float shininess;
}; 
uniform Material material;

// lights
struct Directional_light
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
}; 
uniform Directional_light dir_light;
vec3 directionalLight(Directional_light light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    // specular 
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // results
    vec3 ambient = light.ambient * texture(material.diffuse, texture_coo).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coo).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, texture_coo).rgb;
    return (ambient + diffuse + specular);
}

#define N_PT_LIGHT 4
struct Point_light
{
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
}; 
uniform Point_light pt_lights[N_PT_LIGHT];
vec3 pointLight(Point_light light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position-frag_pos);
    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // attenuation -> F=1/(Kc + Kl·d + Kq·d²)
    float d = length(light.position-frag_pos);
    float attenuation = 
        1.0/(light.constant + light.linear*d + light.quadratic*(d*d));
    // results
    vec3 ambient = light.ambient * texture(material.diffuse,texture_coo).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coo).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, texture_coo).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;
    return (ambient + diffuse + specular);
}

struct Spot_light
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
    float cutoff, outer_cutoff;
}; 
uniform Spot_light spotlight;
vec3 spotLight(Spot_light light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position-frag_pos);
    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // attenuation -> F=1/(Kc + Kl·d + Kq·d²)
    float d, attenuation;
    d           = length(light.position-frag_pos);
    attenuation = 1.0/(light.constant + light.linear*d + light.quadratic*(d*d));
    // spotlight (soft edges) | to make spotlight smooth -> I=(𝜃-𝛾)/𝜀vv
    float theta, epsilon, intensity;
    theta     = dot(light_dir, normalize(-light.direction));
    epsilon   = light.cutoff - light.outer_cutoff;
    intensity = clamp((theta-light.outer_cutoff)/epsilon, 0.0, 1.0);
    // results
    vec3 ambient, diffuse, specular; 
    ambient = light.ambient * texture(material.diffuse,texture_coo).rgb;
    diffuse = light.diffuse * diff * texture(material.diffuse, texture_coo).rgb;
    specular=light.specular * spec * texture(material.specular, texture_coo).rgb;

    diffuse *= intensity;
    specular*= intensity;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;
    return (ambient + diffuse + specular);
}

uniform vec3 cam_pos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view_dir = normalize(cam_pos-frag_pos);

    // directional lighting
    vec3 result = directionalLight(dir_light, norm, view_dir);
    // point lights
    for (int i=0; i<N_PT_LIGHT; i++)
        result += pointLight(pt_lights[i], norm, frag_pos, view_dir);
    // spot light
    result += spotLight(spotlight, norm, frag_pos, view_dir);

    FragColor = vec4(result, 1.0);
}