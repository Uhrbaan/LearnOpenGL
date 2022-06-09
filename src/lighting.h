#ifndef lighting_h
#define lighting_h

#include <cglm/cglm.h>

typedef struct
{
    unsigned int diffuse_map, specular_map, emission_map;
    float shininess;

    unsigned int shader_program;
} Material;

typedef struct
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    unsigned int shader_program;
} Light;

void updateMaterial(Material material);
void updateLight(Light light);

#endif