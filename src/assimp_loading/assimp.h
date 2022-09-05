#ifndef ASSIMP_H
#define ASSIMP_H

#include <assimp/cimport.h>
#include <assimp/scene.h>

unsigned int extractVertices(const struct aiMesh *mesh, float **vertices);
unsigned int extractIndices (const struct aiMesh *mesh, unsigned int **indices);
struct material* extractMaterial(const struct aiMesh *mesh, 
                                 const struct aiScene *scene,
                                 const char *directory);

#endif