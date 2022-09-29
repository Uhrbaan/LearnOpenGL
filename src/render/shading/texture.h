#ifndef TEXTURE_H
#define TEXTURE_H

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