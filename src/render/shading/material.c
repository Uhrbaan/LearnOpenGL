#include <stdio.h>
#include <string.h>

#include "material.h"
#include "../opengl.h"
#include "../../utils/utils.h"

struct material_list material_list = {0};
unsigned int material_uniform = 0;

void initMaterials(unsigned int shader_program)
{
    material_uniform = glGetUniformLocation(shader_program, "material");
    material_list.shader_program = shader_program;
}

int addMaterial(struct material material)
{
    int index = getMaterialIndex(material);
    if (index>=0)
        return index;
    for (int i=0; i<material_list.elements; i++)
    {
        if (!memcmp(&material_list.material[i], &(struct material){0}, 
            sizeof(struct material)))
        {
            memcpy(&material_list.material[i], &material, 
                   sizeof(struct material));
            return i;
        }
    }
    size_t new_size =                                                           // if there is no empty space in the arr
        push((void**)&material_list.material, 
             sizeof(struct material)*material_list.elements,
             sizeof(struct material)*material_list.max_elements,
             (void*)&material, sizeof(struct material));
    material_list.elements++;
    material_list.max_elements = new_size / sizeof(struct material);
    return material_list.elements-1;
}

struct material rmMaterial(int index)
{
    struct material material = {0};

    if (index>=material_list.elements)
    {
        fprintf(stderr, ERROR_MSG_LOG("indexing more than allowed", 
                "index is larger than the number of elements in material_list"));
        return material;
    }
    if (index<0)
    {
        memcpy(&material, &material_list.material[material_list.elements+index+1],
               sizeof(struct material));
        memset(&material_list.material[material_list.elements+index+1], 0, 
               sizeof(struct material));
    }
    else 
    {
        memcpy(&material, &material_list.material[index],
               sizeof(struct material));
        memset(&material_list.material[index], 0, 
               sizeof(struct material));
    }
    return material;
}

int getMaterialIndex(struct material material)
{
    for (int i=0; i<material_list.elements; i++)
    {
        if (!memcmp(&material_list.material[i], &material, 
            sizeof(struct material)))
        return i;
    }
    return -1;
}

void printMaterialList()
{
    printf("\x1b[0;1mMATERIAL LIST\x1b[0;0m\n"
           "elements: %d\t max_elements: %d\n"
           "┌─────────┬──────────┬─────────┬──────────┬───────────┬──────────┐\n"
           "│ diffuse │ specular │ ambient │ emissive │ shininess │ strenght │\n"
           "├─────────┼──────────┼─────────┼──────────┼───────────┼──────────┤\n",
           material_list.elements, material_list.max_elements);
   
    for (int i=0; i<material_list.elements; i++)
    {
        printf("│ %7d │ %8d │ %7d │ %8d │ %9.2f │ %8.2f │\n",
               material_list.material[i].texture_id[1], 
               material_list.material[i].texture_id[2],
               material_list.material[i].texture_id[3],
               material_list.material[i].texture_id[4],
               material_list.material[i].shininess,
               material_list.material[i].strenght);
    }
    printf("└─────────┴──────────┴─────────┴──────────┴───────────┴──────────┘\n");
}