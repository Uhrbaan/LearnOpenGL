#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec3 Normal;
in vec2 texture_coo;
uniform vec3 cam_pos;

struct Material
{
    sampler2D diffuse, specular, emissive;
    float shininess;
}; 
uniform Material material;

// lights
#define DIRECTIONAL_LIGHTS 1
uniform uint directional_light_n = DIRECTIONAL_LIGHTS;
struct Directional_light                                                        // directional light
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
}; 
uniform Directional_light directional_light[DIRECTIONAL_LIGHTS];

#define POINT_LIGHTS 4
uniform uint point_light_n = POINT_LIGHTS;
struct Point_light                                                              // point light
{
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
}; 
uniform Point_light point_light[POINT_LIGHTS];

#define SPOT_LIGHTS 1
uniform uint spot_light_n = SPOT_LIGHTS;
struct Spot_light                                                               // spot light
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
    float cutoff, outer_cutoff;
}; 
uniform Spot_light spot_light[SPOT_LIGHTS];

vec3 directionalLight(Directional_light light, vec3 normal, vec3 view_direction)
{
    vec3 light_dir = normalize(-light.direction);
    vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);

    ambient =                                                                   // ambient
        light.ambient *
        texture(material.diffuse, texture_coo).rgb;

    diffuse =                                                                   // diffuse
        light.diffuse *
        max(dot(normal, light_dir), 0.0) *
        texture(material.diffuse, texture_coo).rgb;

    vec3 reflect_direction = reflect(-light_dir, normal);
    specular =                                                                  // specular
        light.specular *
        pow(max(dot(reflect_direction, view_direction), 0.0), material.shininess) *
        texture(material.specular, texture_coo).rgb;

    return (ambient + diffuse + specular);
}

vec3 pointLight(Point_light light, vec3 normal, vec3 frag_pos, vec3 view_direction)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);

    float d = length(light.position-frag_pos);                                  // light attenuation
    float Kc = light.constant<1.0 ? 1.0 : light.constant;                       // 1.0 / (Kc + Kl∗d + Kq∗d²)
    float attenuation = 
        1.0 / (Kc + light.linear*d + light.quadratic*(d*d));

    ambient =                                                                   // ambient
        light.ambient *
        texture(material.diffuse, texture_coo).rgb *
        attenuation;

    diffuse =                                                                   // diffuse
        light.diffuse *
        max(dot(normal, light_dir), 0.0) *
        texture(material.diffuse, texture_coo).rgb *
        attenuation;

    vec3 reflect_direction = reflect(-light_dir, normal);
    specular =                                                                  // specular
        light.specular *
        pow(max(dot(reflect_direction, view_direction), 0.0), material.shininess) *
        texture(material.specular, texture_coo).rgb *
        attenuation;

    return (ambient + diffuse + specular);
}

vec3 spot_Light(Spot_light light, vec3 normal, vec3 frag_pos, vec3 view_direction)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);

    float d = length(light.position-frag_pos);                                  // light attenuation
    float Kc = light.constant<1.0 ? 1.0 : light.constant;                       // 1.0 / (Kc + Kl∗d + Kq∗d²)
    float attenuation = 
        1.0 / (Kc + light.linear*d + light.quadratic*(d*d));

    float theta, epsilon, intensity;                                            // spot light soft edges intensity
    theta     = dot(light_dir, normalize(-light.direction));                    // I=(𝜃-𝛾)/𝜀
    epsilon   = light.cutoff - light.outer_cutoff;
    intensity = clamp((theta-light.outer_cutoff)/epsilon, 0.0, 1.0);

    ambient =                                                                   // ambient
        light.ambient *
        texture(material.diffuse, texture_coo).rgb *
        attenuation;

    diffuse =                                                                   // diffuse
        light.diffuse *
        max(dot(normal, light_dir), 0.0) *
        texture(material.diffuse, texture_coo).rgb *
        attenuation *
        intensity;

    vec3 reflect_direction = reflect(-light_dir, normal);
    specular =                                                                  // specular
        light.specular *
        pow(max(dot(reflect_direction, view_direction), 0.0), material.shininess) *
        texture(material.specular, texture_coo).rgb *
        attenuation *
        intensity;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view_direction = normalize(cam_pos-frag_pos);

    vec3 result = vec3(0.0);
    // directional lighting
    for (int i=0; i<DIRECTIONAL_LIGHTS; i++)
        result += directionalLight(directional_light[i], norm, view_direction);
    // point lights
    for (int i=0; i<POINT_LIGHTS; i++)
        result += pointLight(point_light[i], norm, frag_pos, view_direction);
    // spot light
    for (int i=0; i<SPOT_LIGHTS; i++)
        result += spot_Light(spot_light[i], norm, frag_pos, view_direction);

    FragColor = vec4(result, 1.0);
}