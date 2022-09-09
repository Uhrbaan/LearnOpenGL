#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "assimp.h"

struct model
{
    struct mesh *meshes; int n_meshes;
    char *directory;
};

struct model loadModel(const char *path, int flags);

#endif