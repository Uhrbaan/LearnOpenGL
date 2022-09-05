#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assimp.h"
#include "../utils/utils.h"

unsigned int extractVertices(const struct aiMesh *mesh, float **vertices)
{
    float *vert = calloc(mesh->mNumVertices, sizeof(float) * 8);                // 3pos + 3norm + 2uv
    for (int i=0; i<mesh->mNumVertices; i++)
    {
        vert[i*8+0] = mesh->mVertices[i].x;
        vert[i*8+1] = mesh->mVertices[i].y;
        vert[i*8+2] = mesh->mVertices[i].z;

        vert[i*8+3] = mesh->mNormals[i].x;
        vert[i*8+4] = mesh->mNormals[i].y;
        vert[i*8+5] = mesh->mNormals[i].z;

        vert[i*8+6] = mesh->mTextureCoords[0][i].x;
        vert[i*8+7] = mesh->mTextureCoords[0][i].y;
    }
    *vertices = vert;
    return mesh->mNumVertices;
}

unsigned int extractIndices(const struct aiMesh *mesh, unsigned int **indices)
{
    unsigned int n_indi = 0, *indi;
    for (int i=0; i<mesh->mNumFaces; i++)                                       // counting indices
        n_indi += mesh->mFaces[i].mNumIndices;
    indi = calloc(n_indi, sizeof(unsigned int));

    unsigned int k=0;
    for (int i=0; i<mesh->mNumFaces; i++)
        for (int j=0; j<mesh->mFaces[i].mNumIndices; j++)
            indi[k++] = mesh->mFaces[i].mIndices[j];
    
    *indices = indi;
    return n_indi;
}

#include "../render/shading.h"
#include "../render/opengl.h"
// TODO support multiple textures per textures type
struct material *extractMaterial(const struct aiMesh *mesh, 
                                 const struct aiScene *scene,
                                 const char *directory)
{
    if (!mesh->mMaterialIndex)
    {
        fprintf(stderr, TERM_COL_ERROR("error: ")
                        "no material for the mesh %p", mesh);
        return NULL;
    }
    // loading textures
    struct aiMaterial   *ai_mat = scene->mMaterials[mesh->mMaterialIndex];
    struct aiString      ai_str = {0};
    struct material    material = {0};
    char texture_path[PATH_MAX] = {0};
    unsigned int max;
    for (int i=1; i<SUPPORTED_TEXTURE_TYPES_N; i++)
    {
        ai_str.data[0] = '\0'; // terminate str through first char
        aiGetMaterialTexture(ai_mat, i, 0, &ai_str,                             // get texture filename 
                             NULL, NULL, NULL, NULL, NULL, NULL);
        if (ai_str.data[0] != '\0') // there is such texture file
        {
            snprintf(texture_path, PATH_MAX, "%s/%s", directory, ai_str.data);    // filepath of the texture
            int texture_index = 
                addTexture(i, 0, texture_path);
            if (!texture_list.texture_id[texture_index])                        // set glid if texture didn't exist
                texture_list.texture_id[texture_index] = 
                    loadGLTexture(texture_path);
            printTextureList();

            // uniform location
            char uniform_name[100]={0};
            snprintf(uniform_name, 100, "material.textures[%d]", i);

            material.texture_id[i] = texture_list.texture_id[texture_index];
            material.texture_uniform[i] = 
                glGetUniformLocation(material_list.shader_program, uniform_name);
        }
    }

    max = 1;                                                                    // shininess & shininess_strength
    aiGetMaterialFloatArray(ai_mat, AI_MATKEY_SHININESS,
        (float*)&material.shininess, &max);
    max = 1;
    aiGetMaterialFloatArray(ai_mat, AI_MATKEY_SHININESS_STRENGTH,
        (float*)&material.strenght, &max);

    int material_index = addMaterial(material);
    printMaterialList();
    return &material_list.material[material_index];
}