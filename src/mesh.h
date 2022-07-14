#ifndef mesh_h
#define mesh_h

#include "gl.h"
#include <cglm/cglm.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/material.h>

struct vertex {vec3 position, normal; vec2 texure_coo;};
struct texture 
{
    unsigned int id;
    char *type; // diffuse/specular/...
    char *path; // to be able to check if already loaded
}; 
typedef struct mesh
{
    struct vertex *vertices;  int n_vert;
    unsigned int *indices;    int n_ind;
    // stored in the loaded textures (static in model.c -> dynamic array)
    struct texture *textures; int n_tex;
    // rendering data
    unsigned int vao, vbo, ebo;    
} Mesh;

Mesh generateMesh(struct aiMesh *mesh, const struct aiScene *scene, const char *directory);
void initMesh(Mesh *mesh);
void drawMesh(Mesh *mesh, unsigned int shader_program);

#endif