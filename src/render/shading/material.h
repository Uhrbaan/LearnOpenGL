#ifndef MATERIAL_H
#define MATERIAL_h

#include "../../utils/texture_types.h"

struct material
{
    unsigned int texture_id     [MAX_TEXTURE_TYPES],
                 texture_uniform[MAX_TEXTURE_TYPES];
    float shininess, strenght;
};
struct material_list
{
    unsigned int elements, max_elements, shader_program;
    struct material *material;
};

extern struct material_list material_list;
void initMaterials(unsigned int shader_program);
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

#endif