#include "model.h"
#include "../utils/utils.h"
#include <assimp/postprocess.h>
#include <libgen.h> // for dirname

struct model loadModel(const char *path)
{
    unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs;
    const struct aiScene *scene = aiImportFile(path, flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        scene->mNumMeshes <= 0)
    {
        aiReleaseImport(scene);
        fprintf(stderr, ERROR_MSG_LOG("assimp could not load file", 
                                       aiGetErrorString()));
        return (struct model){0};
    }
    struct model model={0};
    char directory[1048];
    strcpy(directory, path);
    dirname(directory);

    model.directory = malloc(sizeof(char)*strlen(directory));
    strcpy(model.directory, directory);

    model.n_meshes = scene->mNumMeshes;
    model.meshes = malloc(sizeof(struct mesh)*model.n_meshes);

    // reading the meshes
    for (int i=0; i<model.n_meshes; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[i];
        model.meshes[i] = generateMesh(mesh, scene, directory);
    }
    return model;
}

void drawModel(struct model m, unsigned int shader_program)
{
    for (int i=0; i<m.n_meshes; i++)
        drawMesh(m.meshes[i], shader_program);
}