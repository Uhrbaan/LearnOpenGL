#include "model.h"
#include "../utils/utils.h"
#include <libgen.h> // for dirname

struct model loadModel(const char *path, int flags)
{
    const struct aiScene *scene = loadScene(path, flags);                             // loading assimp scene

    if (!scene) return (struct model){0};

    struct model model={0};
    char directory[1048];
    strcpy(directory, path);
    dirname(directory);

    model.directory = malloc(sizeof(char)*strlen(directory));
    strcpy(model.directory, directory);

    model.n_meshes = scene->mNumMeshes;
    model.meshes = malloc(sizeof(struct mesh)*model.n_meshes);

    for (int i=0; i<model.n_meshes; i++)                                        // getting mesh information
    {
        struct aiMesh *ai_mesh = scene->mMeshes[i];
        model.meshes[i] = generateMesh(ai_mesh, scene, directory);
    }

    printMaterialList();
    printTextureList();

    return model;
}

void drawModel(struct model m, unsigned int shader_program)
{
    for (int i=0; i<m.n_meshes; i++)
        drawMesh(m.meshes[i], shader_program);
}