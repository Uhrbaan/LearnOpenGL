#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assimp.h"
#include "../utils/utils.h"

const struct aiScene *loadScene(const char *path, int flags)
{
    const struct aiScene *scene = aiImportFile(path, flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        scene->mNumMeshes <= 0)
    {
        aiReleaseImport(scene);
        fprintf(stderr, ERROR_MSG_LOG("assimp could not load file", "%s"),
                                       aiGetErrorString());
        return NULL;
    }
    return scene;
}

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
struct material extractMaterial(const struct aiMesh *mesh, 
                                 const struct aiScene *scene,
                                 const char *directory,
                                 unsigned int *material_list_index)
{
    if (!mesh->mMaterialIndex)
    {
        fprintf(stderr, TERM_COL_ERROR("error: ")
                        "no material for the mesh %p", mesh);
        return (struct material){0};
    }
    // loading textures
    struct aiMaterial   *ai_mat = scene->mMaterials[mesh->mMaterialIndex];
    struct aiString      ai_str = {0};
    struct material    material = {0};
    char texture_path[PATH_MAX] = {0};
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

            // uniform location
            char uniform_name[100]={0};
            snprintf(uniform_name, 100, "material.textures[%d]", i);

            material.texture_id[i] = texture_list.texture_id[texture_index];
            material.texture_uniform[i] = 
                glGetUniformLocation(material_list.shader_program, uniform_name);
        }
    }
/* TODO add shininess values to models, right now set to static 32
    float shininess; 
    aiGetMaterialFloatArray(ai_mat, AI_MATKEY_SHININESS,
        (float*)&shininess, (unsigned int *)0x0);
    material.shininess = shininess * (128.f/4000.f);                            // correct range of wavefront 
                                                                                // [0, 4000] -> [0, 128]
    // aiGetMaterialFloatArray(ai_mat, AI_MATKEY_SHININESS_STRENGTH, -> does not exist in mtl
        // (float*)&material.strenght, (unsigned int *)0x0);
 */

    unsigned int material_index = addMaterial(material);
    if (material_list_index!=NULL) *material_list_index = material_index;
    
    return material;
}

// TODO handle object with less information