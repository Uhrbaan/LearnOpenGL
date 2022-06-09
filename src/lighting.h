#ifndef lighting_h
#define lighting_h

#include <cglm/cglm.h>

typedef struct
{
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    unsigned int shader_program;
} Material;

typedef struct
{
    vec3 position;
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;

    unsigned int shader_program;
} Light;


void updateMaterial(Material material);
void updateLight(Light light);

#endif