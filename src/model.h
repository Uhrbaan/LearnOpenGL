#ifndef model_h
#define model_h

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

typedef struct
{
    Mesh *meshes; int n_mesh; // malloced to scene->RootNode->mNumChildren
    char *directory; // malloced
} Model;

int loadModel(Model *model, const char *path);
void drawModel(Model *model, unsigned int shader_program);

// void processNode(Model *model, aiNode *node, const aiScene *scene);
// Mesh processMesh(aiMesh *mesh, const aiScene *scene, const char *directory);

#endif