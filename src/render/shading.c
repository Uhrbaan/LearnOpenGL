#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "opengl.h"
#include "shading.h"

unsigned int material_uniform= 0;

struct directional_light *directional_light = NULL;
unsigned int directional_light_n = 0;
static struct 
{ 
    unsigned int direction, ambient, diffuse, specular; 
} *directional_light_uniform = NULL;
    
struct point_light *point_light = NULL;
unsigned int point_light_n = 0;
static struct 
{
    unsigned int position, ambient, diffuse, specular, constant, 
                 linear, quadratic;                             
} *point_light_uniform = NULL;

struct spot_light *spot_light = NULL;
unsigned int spot_light_n = 0;
static struct 
{ 
    unsigned int position, direction, ambient, diffuse, specular,
                 constant, linear, quadratic, cutoff, outer_cutoff;
} *spot_light_uniform = NULL;

/******************************** initShading *********************************/

void initShading(unsigned int shader_program)
{
    int i; char uniform_name[100];
    // find the number of each type of light
    material_uniform = glGetUniformLocation(shader_program, "material");
    material_list.shader_program = shader_program;

    glGetUniformuiv(                                                            // directional light
        shader_program, 
        glGetUniformLocation(shader_program, "directional_light_n"),
        &directional_light_n);

    directional_light_uniform =
        calloc(directional_light_n, sizeof(*directional_light_uniform));
    assert(directional_light_uniform);

    for (i=0; i<directional_light_n; i++)
    {
        snprintf(uniform_name, 99, "directional_light[%d].direction", i);
        directional_light_uniform[i].direction = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].ambient", i);
        directional_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].diffuse", i);
        directional_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].specular", i);
        directional_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
    }
    directional_light = 
        calloc(directional_light_n, sizeof(struct directional_light));
    assert(directional_light);
    for (i=0; i<directional_light_n; i++) updateDirectionalLight(i, shader_program);


    glGetUniformuiv(                                                            // point light
        shader_program, 
        glGetUniformLocation(shader_program, "point_light_n"),
        &point_light_n);

    point_light_uniform = 
        calloc(point_light_n, sizeof(*point_light_uniform));
    assert(point_light_uniform);

    for (i=0; i<point_light_n; i++)
    {
        snprintf(uniform_name, 99, "point_light[%d].position", i);
        point_light_uniform[i].position = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].ambient", i);
        point_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].diffuse", i);
        point_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].specular", i);
        point_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].constant", i);
        point_light_uniform[i].constant = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].linear", i);
        point_light_uniform[i].linear = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].quadratic", i);
        point_light_uniform[i].quadratic = 
            glGetUniformLocation(shader_program, uniform_name);
    }
    point_light = 
        calloc(point_light_n, sizeof(struct point_light));
    assert(point_light);
    for (i=0; i<directional_light_n; i++) updatePointLight(i, shader_program);


    glGetUniformuiv(                                                            // spot light
        shader_program, 
        glGetUniformLocation(shader_program, "spot_light_n"),
        &spot_light_n);

    spot_light_uniform = 
        calloc(spot_light_n, sizeof(*spot_light_uniform));
    assert(point_light_uniform);

    for (i=0; i<spot_light_n; i++)
    {
        snprintf(uniform_name, 100, "spot_light[%d].position", i);
        spot_light_uniform[i].position = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].direction", i);
        spot_light_uniform[i].direction = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].ambient", i);
        spot_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].diffuse", i);
        spot_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].specular", i);
        spot_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].constant", i);
        spot_light_uniform[i].constant = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].linear", i);
        spot_light_uniform[i].linear = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].quadratic", i);
        spot_light_uniform[i].quadratic = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].cutoff", i);
        spot_light_uniform[i].cutoff = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].outer_cutoff", i);
        spot_light_uniform[i].outer_cutoff = 
            glGetUniformLocation(shader_program, uniform_name);
    }
    spot_light = 
        calloc(spot_light_n, sizeof(struct spot_light));
    assert(spot_light);
    for (i=0; i<directional_light_n; i++) updateSpotLight(i, shader_program);
}

/******************************* update Lights ********************************/

void updateDirectionalLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(directional_light_uniform[index].direction, 1, 
                 directional_light[index].direction);
    glUniform3fv(directional_light_uniform[index].ambient, 1, 
                 directional_light[index].ambient);
    glUniform3fv(directional_light_uniform[index].diffuse, 1, 
                 directional_light[index].diffuse);
    glUniform3fv(directional_light_uniform[index].specular, 1, 
                 directional_light[index].specular);
}

void updatePointLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(point_light_uniform[index].position, 1, 
                 point_light[index].position);
    glUniform3fv(point_light_uniform[index].ambient, 1, 
                 point_light[index].ambient);
    glUniform3fv(point_light_uniform[index].diffuse, 1, 
                 point_light[index].diffuse);
    glUniform3fv(point_light_uniform[index].specular, 1, 
                 point_light[index].specular);
    glUniform1f (point_light_uniform[index].constant,
                 point_light[index].constant);
    glUniform1f (point_light_uniform[index].linear,
                 point_light[index].linear);
    glUniform1f (point_light_uniform[index].quadratic,
                 point_light[index].quadratic);
}
void updateSpotLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(spot_light_uniform[index].position, 1, 
                 spot_light[index].position);
    glUniform3fv(spot_light_uniform[index].direction, 1, 
                 spot_light[index].direction);
    glUniform3fv(spot_light_uniform[index].ambient, 1, 
                 spot_light[index].ambient);
    glUniform3fv(spot_light_uniform[index].diffuse, 1, 
                 spot_light[index].diffuse);
    glUniform3fv(spot_light_uniform[index].specular, 1, 
                 spot_light[index].specular);
    glUniform1f (spot_light_uniform[index].constant,
                 spot_light[index].constant);
    glUniform1f (spot_light_uniform[index].linear,
                 spot_light[index].linear);
    glUniform1f (spot_light_uniform[index].quadratic,
                 spot_light[index].quadratic);
    glUniform1f (spot_light_uniform[index].cutoff,
                 spot_light[index].cutoff);
    glUniform1f (spot_light_uniform[index].outer_cutoff,
                 spot_light[index].outer_cutoff);
}

/******************************* material list ********************************/

#include "../utils/utils.h"
struct material_list material_list = {0};                                       // <materials>

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

/***************************** texture list ***********************************/

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

/******************************* Create Lights ********************************/

void createDirectionalLight(int index, 
                            vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular)
{
    glm_vec3_copy(direction,directional_light[index].direction);
    glm_vec3_copy(ambient,  directional_light[index].ambient);
    glm_vec3_copy(diffuse,  directional_light[index].diffuse);
    glm_vec3_copy(specular, directional_light[index].specular);
}

void createPointLight(int index, 
                      vec3 pos, vec3 ambient, vec3 diffuse, vec3 specular,
                      float linear, float quadratic)
{
    glm_vec3_copy(pos,      point_light[index].position);
    glm_vec3_copy(ambient,  point_light[index].ambient);
    glm_vec3_copy(diffuse,  point_light[index].diffuse);
    glm_vec3_copy(specular, point_light[index].specular);
    point_light[index].constant  = 1.0f;
    point_light[index].linear    = linear;
    point_light[index].quadratic = quadratic;
}

void createSpotLight(int index, 
                     vec3 pos, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular,
                     float linear, float quadratic,
                     float cutoff_deg, float outer_cutoff_deg)
{
    glm_vec3_copy(pos,       spot_light[index].position);
    glm_vec3_copy(direction, spot_light[index].direction);
    glm_vec3_copy(ambient,   spot_light[index].ambient);
    glm_vec3_copy(diffuse,   spot_light[index].diffuse);
    glm_vec3_copy(specular,  spot_light[index].specular);
    spot_light[index].constant  = 1.0f;
    spot_light[index].linear    = linear;
    spot_light[index].quadratic = quadratic;
    spot_light[index].cutoff       = cos(glm_rad(cutoff_deg));
    spot_light[index].outer_cutoff = cos(glm_rad(outer_cutoff_deg));
}