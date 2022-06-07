#version 460 core
out vec4 FragColor;

in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;

uniform vec3 object_color;
uniform vec3 light_color;

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

    //specular -> viewspace
    float specular_strenght = 1.0, shininess = 256;
    vec3 light_dir   = normalize(light_pos-frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    vec3 view_dir    = normalize(-frag_pos);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 specular = light_color*spec;

    vec3 result = (diffuse + ambiant + specular) * object_color;
    FragColor = vec4(result, 1.0);
}