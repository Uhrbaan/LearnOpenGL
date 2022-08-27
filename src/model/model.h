#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>
#include "mesh.h"

struct model
{
    struct mesh *meshes; int n_meshes;
    char *directory;
};

struct model loadModel(const char *path);

#endif