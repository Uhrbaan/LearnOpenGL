#ifndef model_h
#define model_h

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

typedef struct aiNode aiNode;
typedef struct aiScene aiScene;
typedef struct aiMesh aiMesh;
typedef struct aiMaterial aiMaterial;
typedef struct aiString aiString;
typedef enum aiTextureType aiTextureType;

typedef struct
{
    Mesh *meshes; int nm; // malloced to scene->RootNode->mNumChildren
    char *directory; // malloced
} Model;

struct loaded_textures
{
    size_t n;   // number of elements
    size_t n_tot; // total size of the dynamic array
    struct texture *textures; // data
};

void loadModel(Model *model, const char *path);
void drawModel(Model *model, unsigned int shader_program);

void processNode(Model *model, aiNode *node, const aiScene *scene);
Mesh processMesh(aiMesh *mesh, const aiScene *scene);
struct texture *loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                     char *typeName);

#endif