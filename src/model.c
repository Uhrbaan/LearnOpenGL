#include "utils/utils.h"
#include "model.h"
#include "gl.h"
#include <string.h>

#include <libgen.h> // to get the basename / directory of path
int loadModel(Model *model, const char *path)
// checkout https://gist.github.com/tilkinsc/661d9da4a4121101a182b8880857f6fb
// pure C loading w/ assimp
{
    /* creates the scene (root of the data struct) from a file
     * other useful flags:
     *  - aiProcess_GenNormals
     *  - aiProcess_SplitLargeMeshes
     *  - aiProcess_OptimizeMeshes
     * -> documentation  assimp.sourceforge.net/lib_html/postprocess_8h.html  */
    unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs;
    const struct aiScene *scene = aiImportFile(path, flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mNumMeshes <= 0)
    {
        aiReleaseImport(scene);
        fprintf(stderr, ERROR_MSG_LOG("assimp could not load file", 
                                       aiGetErrorString()));
        return 1;
    }
    char directory[1048];
    strcpy(directory, path);
    dirname(directory);

    model->directory = malloc(sizeof(char)*strlen(directory));
    strcpy(model->directory, directory);

    model->n_mesh = scene->mNumMeshes;
    model->meshes = malloc(sizeof(Mesh)*model->n_mesh);

    // reading the meshes
    for (int i=0; i<model->n_mesh; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[i];
        model->meshes[i] = generateMesh(mesh, scene, directory);
    }
    return 1;
}

void drawModel(Model *model, unsigned int shader_program)
{
    for (int i=0; i<model->n_mesh; i++)
        drawMesh(&model->meshes[i], shader_program);
}