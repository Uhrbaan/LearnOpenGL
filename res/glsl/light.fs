#version 460 core
out vec4 FragColor;

in vec3 normal;
in vec3 frag_pos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

void main()
{
    // ambiant
    float ambiant_strength = 0.3; // ambiant lighting
    vec3 ambiant = light_color*ambiant_strength;

    // diffuse
    vec3 norm = normalize(normal); // norm because only direction is important
    vec3 light_direction = normalize(light_pos-frag_pos);
    /* getting brightness value by calculating the dot product between the light
     * direction & the cube's face normal. 
     * the dot product approaches 0 as angle -> 90° */
    float diff = max(dot(norm, light_direction), 0.0); 
    vec3 diffuse = diff*light_color;

    // specular
    float specular_strength = 0.3;
    vec3 cam_direction = normalize(cam_pos-frag_pos);
    vec3 reflect_dir = reflect(-light_direction, norm);
    // the pow (val, n) where n is the shininess
    float spec = pow(max(dot(cam_direction, reflect_dir), 0.0), 256);
    vec3 specular = specular_strength*spec*light_color;

    vec3 result = (diffuse + ambiant + specular) * object_color;
    FragColor = vec4(result, 1.0);
}