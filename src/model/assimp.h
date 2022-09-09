#ifndef ASSIMP_H
#define ASSIMP_H

#include <assimp/cimport.h>
#include <assimp/scene.h>

/**
 * @brief loads the assimp scene form path
 * 
 * @param path path to the .obj file
 * @param flags assimp process flags such as
 *              aiProcess_Triangulate, aiProcess_FlipUVs, aiProcess_GenNormals,
 *              aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
 * @return pointer to the scene or NULL if failed
 */
const struct aiScene *loadScene(const char *path, int flags);
unsigned int extractVertices(const struct aiMesh *mesh, float **vertices);
unsigned int extractIndices (const struct aiMesh *mesh, unsigned int **indices);
struct material* extractMaterial(const struct aiMesh *mesh, 
                                 const struct aiScene *scene,
                                 const char *directory);

#endif