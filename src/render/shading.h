#ifndef SHADING_H
#define SHADING_H

#include <cglm/cglm.h>

// copy of the glsl structs
struct material                                                                 // material
{
    unsigned int diffuse, specular, emission;
    float shininess;
};
extern struct material material;

struct directional_light                                                        // directional light
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
};
extern unsigned int directional_light_n;
extern struct directional_light *directional_light;

struct point_light                                                              // point light
{
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
};
extern unsigned int point_light_n;
extern struct point_light *point_light;

struct spot_light                                                               // spot light
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
    float cutoff, outer_cutoff;
};
extern unsigned int spot_light_n;
extern struct spot_light *spot_light;

void initShading(unsigned int shader_program);
void updateDirectionalLight (unsigned int index);
void updatePointLight       (unsigned int index);
void updatespotLight        (unsigned int index);

#endif