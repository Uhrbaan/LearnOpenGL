#ifndef lighting_h
#define lighting_h

#include <cglm/cglm.h>

enum EnumLIGHT
{
    DIRECTIONAL,
    POINT,
    SPOT
};

typedef struct
{
    unsigned int diffuse_map, specular_map, emission_map;
    float shininess;

    unsigned int shader_program;
} Material;

typedef struct 
{
    vec3 direction;
    vec3 ambient, diffuse, specular;

    unsigned int shader_program;
    unsigned int arr[4];
} Directional_light; 

typedef struct 
{
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;

    unsigned int shader_program;
    unsigned int arr[7];
} Point_light; 

typedef struct 
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
    float cutoff, outer_cutoff;

    unsigned int shader_program;
    unsigned int arr[10];
} Spot_light; 

void updateMaterial(Material material);
// returns the uniform locations in order
void updateLight(void* p, const char *uniform_name, enum EnumLIGHT type);
#endif
