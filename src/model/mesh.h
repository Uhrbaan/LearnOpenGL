#ifndef MESH_H
#define MESH_H

#include <cglm/cglm.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

struct vertex  {vec3 pos, norm; vec2 uv;};
struct texture {unsigned int gl_id; unsigned int type; char path[100];};
struct mesh
{
    struct vertex  *vertices; int n_vert;
    unsigned int   *indices;  int n_indi;
    struct texture **textures; int n_text; // textures live in another array
    unsigned int vao, vbo, ebo;
};

// dont forget to generate vao afterwards
struct mesh generateMesh(struct aiMesh *mesh, const struct aiScene *scene, 
                         const char *directory);
void setGlTextureId_fn(unsigned int (*fn)(const char*));

#endif