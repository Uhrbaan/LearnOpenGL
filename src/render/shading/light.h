#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/cglm.h>

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS       4
#define MAX_SPOT_LIGHTS        1

struct directional_light                                                        // directional light
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
};
struct point_light                                                              // point light
{
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
};
struct spot_light                                                               // spot light
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
    float cutoff, outer_cutoff;
};

extern struct directional_light directional_light[MAX_DIRECTIONAL_LIGHTS];
extern struct point_light       point_light      [MAX_POINT_LIGHTS];
extern struct spot_light        spot_light       [MAX_SPOT_LIGHTS];
extern unsigned int directional_light_n;
extern unsigned int point_light_n;
extern unsigned int spot_light_n;

void initLights(unsigned int shader_program);
void createDirectionalLight(int index, vec3 direction, 
                            vec3 ambient, vec3 diffuse, vec3 specular);
void createPointLight      (int index, 
                            vec3 pos, vec3 ambient, vec3 diffuse, vec3 specular,
                            float linear, float quadratic);
void createSpotLight       (int index, vec3 pos, vec3 direction, 
                            vec3 ambient, vec3 diffuse, vec3 specular,
                            float linear, float quadratic,
                            float cutoff_deg, float outer_cutoff_deg);
void updateDirectionalLight(unsigned int index, unsigned int shader_program);
void updatePointLight      (unsigned int index, unsigned int shader_program);
void updateSpotLight       (unsigned int index, unsigned int shader_program);

#endif

/********************************* NOTES ***************************************

# light propagation
constant is usually kept at 1
linear should be small if light reaches long distances
quadratic even smaller
formula: attenuation = 1.0 / (Kc + Kl∗d + Kq∗d²)
# example table of different values
| Distance  |  Constant  |  Linear  |  Quadratic  |
|-----------|------------|----------|-------------|
| 7         |  1.0       |  0.7     |  1.8        |
| 13        |  1.0       | 0.35     |  0.44       |
| 20        |  1.0       | 0.22     |  0.20       |
| 32        |  1.0       | 0.14     |  0.07       |
| 50        |  1.0       | 0.09     |  0.032      |
| 65        |  1.0       | 0.07     |  0.017      |
| 100       |  1.0       | 0.045    |  0.0075     |
| 160       |  1.0       | 0.027    |  0.0028     |
| 200       |  1.0       | 0.022    |  0.0019     |
| 325       |  1.0       | 0.014    |  0.0007     |
| 600       |  1.0       | 0.007    |  0.0002     |
| 3250      |  1.0       | 0.0014   |  0.000007   |

*/