#ifndef SHADING_H
#define SHADING_H

#include <cglm/cglm.h>

// copy of the glsl structs
struct directional_light                                                        // directional light
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
};
extern unsigned int directional_light_n;
extern struct directional_light *directional_light;

/**
 * constant is usually kept at 1
 * linear should be small if light reaches long distances
 * quadratic even smaller
 * formula: attenuation = 1.0 / (Kc + Kl∗d + Kq∗d²)
 * # example table of different values
 * | Distance  |  Constant  |  Linear  |  Quadratic  |
 * |-----------|------------|----------|-------------|
 * | 7         |  1.0       |  0.7     |  1.8        |
 * | 13        |  1.0       | 0.35     |  0.44       |
 * | 20        |  1.0       | 0.22     |  0.20       |
 * | 32        |  1.0       | 0.14     |  0.07       |
 * | 50        |  1.0       | 0.09     |  0.032      |
 * | 65        |  1.0       | 0.07     |  0.017      |
 * | 100       |  1.0       | 0.045    |  0.0075     |
 * | 160       |  1.0       | 0.027    |  0.0028     |
 * | 200       |  1.0       | 0.022    |  0.0019     |
 * | 325       |  1.0       | 0.014    |  0.0007     |
 * | 600       |  1.0       | 0.007    |  0.0002     |
 * | 3250      |  1.0       | 0.0014   |  0.000007   |
 */
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

void initShading            (unsigned int shader_program);
void updateDirectionalLight (unsigned int index, unsigned int shader_program);
void updatePointLight       (unsigned int index, unsigned int shader_program);
void updatespotLight        (unsigned int index, unsigned int shader_program);

#define SUPPORTED_TEXTURE_TYPES_N 5 // max 20
struct material                                                                 // material
{
    unsigned int texture_id[SUPPORTED_TEXTURE_TYPES_N],                         // diffuse, specular, ambient, emissive;
                 texture_uniform[SUPPORTED_TEXTURE_TYPES_N];
    float shininess, strenght;
};
extern struct material material; // does it have any use ?
struct material_list
{
    unsigned int elements, max_elements, shader_program;
    struct material *material;
};
extern struct material_list material_list;
/**
 * @brief adds a material to an empty spot in the array, or returns index if 
 *        already exists
 * 
 * @param material to put on the array
 * @return index to the material
 */
int addMaterial(struct material material);
/**
 * @brief removes the material at index, sets to 0
 * 
 * @param index to be removed, if negative starts from last element
 * @return structure that was removed
 */
struct material rmMaterial(int index);
/**
 * @brief Get the the index of specified material
 *
 * @param material: search argument
 * @return index of the material, -1 if not found
 */
int getMaterialIndex(struct material material);
void printMaterialList();

struct texture_list
{
    unsigned int  elements, max_elements;
    unsigned int *texture_type;
    unsigned int *texture_id;
    char        **texture_path;
};
extern struct texture_list texture_list;
/**
 * @brief add texture to empty spot on the list
 * 
 * @param texture_type 
 * @param texture_id GL id of the texture
 * @param texture_path path to the texture file
 * @return index of the texture
 */
int addTexture (unsigned int texture_type, unsigned int texture_id, 
                const char *texture_path);
/**
 * @brief removes a texture from a list, sets index to 0
 * 
 * @param index index to be removed
 * @param texture_type returns texture type of the removed texture
 * @param texture_id returns the texture id of the removed texture, if not 0,
 *                   can be a search argument if index is not given 
 * @param texture_path returns the texture path of the removed texture, if not
 *                     0, can be a search argument if index & texture_id are not
 *                     given
 */
void rmTexture (int index, unsigned int *texture_type, unsigned int *texture_id,
                char **texture_path);
/**
 * @brief Get the Texture object
 * 
 * @param index index of the texture, -1 starts from the end, NULL ignored
 * @param texture_type search argument
 * @param texture_id search argument if index not given
 * @param texture_path search argument if index or id not given
 * 
 * @return index to the texture, -1 if fail, referenced variables will be set 
 *         with the value of the found texture
 */
int getTexture(int *index, unsigned int *texture_type, 
               unsigned int *texture_id, char **texture_path);
void printTextureList();

#endif

// TODO include shininess & shininess strnegth into material