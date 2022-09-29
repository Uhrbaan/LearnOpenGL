#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "../../utils/utils.h"
#include "../../utils/texture_types.h"

struct texture_list texture_list = {0};

int addTexture (unsigned int texture_type, unsigned int texture_id, 
                const char *texture_path)
{
    int index;
    char *tpath = calloc(strlen(texture_path)+1, 1);
    strcpy(tpath, texture_path);

    index = getTexture(NULL, &texture_type, &texture_id, &tpath);        // check if texture already exists
    if (index>=0)
        return index; 
    for (int i=0; i<texture_list.elements; i++)
    {
        if (texture_list.texture_id[i] == 0)                                    // if the glid is 0, it means that the 
        {                                                                       // texture is invalid anyway
            texture_list.texture_type[i] = texture_type;
            texture_list.texture_id[i]   = texture_id;
            texture_list.texture_path[i] = tpath;
            return i;
        }   
    }                                                                           // have to generate bigger array

    push((void**)&texture_list.texture_type, 
         sizeof(unsigned int) * texture_list.elements,
         sizeof(unsigned int) * texture_list.max_elements,
         (void*)&texture_type, sizeof(unsigned int));

    push((void**)&texture_list.texture_id,
         sizeof(unsigned int) * texture_list.elements,
         sizeof(unsigned int) * texture_list.max_elements,
         (void*)&texture_id, sizeof(unsigned int));
    
    size_t new_size = 
        push((void**)&texture_list.texture_path, 
            sizeof(char *) * texture_list.elements,
            sizeof(char *) * texture_list.max_elements,
            (void*)&tpath, sizeof(char *));

    texture_list.elements++;
    texture_list.max_elements = new_size / sizeof(char *);
    return texture_list.elements-1
    ;
}

void rmTexture (int index, unsigned int *texture_type, unsigned int *texture_id,
                char **texture_path)
{
    if (index>=texture_list.elements)
    {
        fprintf(stderr, ERROR_MSG_LOG("indexing more than allowed", 
                "index is larger than the number of elements in texture_list"));
    }
    if (index<0)
        index = texture_list.elements + index +1;

    if (texture_type)
        *texture_type = texture_list.texture_type[index];
    if (texture_id)
        *texture_id   = texture_list.texture_id[index];
    if (texture_path)
    {
        char *tpath = calloc(strlen(texture_list.texture_path[index]+1), 1);
        strcpy(tpath, texture_list.texture_path[index]);
        *texture_path = tpath;
    }

    texture_list.texture_type[index] = 0;                                       // set to 0
    texture_list.texture_id[index]   = 0;
    free(texture_list.texture_path[index]);
    texture_list.texture_path[index] = NULL;
}

int getTexture(int *index, unsigned int *texture_type, 
                        unsigned int *texture_id, char **texture_path)
{
    int i=0;
    if (index != NULL)
    {
        if (*index>=0)
        {
            i = *index;
            goto set_values;
        }
        else if (*index<0)
        {
            i = texture_list.elements + *index +1;
            goto set_values;
        }
    }
    for (i=0; i<texture_list.elements; i++)
    {
        if (texture_id!=NULL && *texture_id)
        {
            if (*texture_id==texture_list.texture_id[i])
                goto set_values;
        }
        else if (texture_path!=NULL && *texture_path[0])
            if (!strncmp(texture_list.texture_path[i], *texture_path, PATH_MAX))
                goto set_values;                
    }
    return -1;
    
    set_values:
        if (index!=NULL)        *index = i;
        if (texture_type!=NULL) *texture_type = texture_list.texture_type[i];
        if (texture_id!=NULL)   *texture_id   = texture_list.texture_id[i];
        if (texture_path!=NULL && !*texture_path[0])
        {
            int texture_path_len = strlen(texture_list.texture_path[i]);
            texture_list.texture_path[i] =
                realloc(*texture_path, texture_path_len);
            strncpy(*texture_path, texture_list.texture_path[i], PATH_MAX);
        }
        return i;

}

void printTextureList()
{
    printf("\x1b[0;1mTEXTURE LIST\x1b[0;0m\n"
           "elements: %d\tmax_elements: %d\n"
           "┌──────────────┬────────────┬──────────────┐\n"
           "│ texture type │ texture id │ texture path │\n"
           "├──────────────┼────────────┼──────────────┤\n",
           texture_list.elements, texture_list.max_elements);

    char type[20], id[20];
    
    for (int i=0; i<texture_list.elements; i++)
    {
        switch (texture_list.texture_type[i])
        {
            case none:
                strcpy(type, "none");
                break;
            case diffuse:
                strcpy(type, "diffuse");
                break;
            case specular:
                strcpy(type, "specular");
                break;
            case ambient:
                strcpy(type, "ambient");
                break;
            case emissive:
                strcpy(type, "emissive");
                break;
            
            default:
                break;
        }
        sprintf(id, "%d", texture_list.texture_id[i]);
        printf("│ %12s │ %10s │ %-12s │\n", type, id, texture_list.texture_path[i]);
    }
    printf("└──────────────┴────────────┴──────────────┘\n");
}
