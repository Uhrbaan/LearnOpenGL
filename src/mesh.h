#ifndef mesh_h
#define mesh_h

#include <cglm/cglm.h>

struct vertex {vec3 position, normal, texure_coo;};
struct texture 
{
    unsigned int id;
    char *type; // diffuse/specular/...
    char *path; // to be able to check if already loaded
};
typedef struct mesh
{
    // mesh data
    struct vertex *vertices;  int vn;
    unsigned int *indices;    int in;
    struct texture *textures; int tn;
    // rendering data
    unsigned int vao, vbo, ebo;    
} Mesh;

Mesh createMesh(struct vertex *vertices, int vn, 
                unsigned int *indices, int in,
                struct texture *textures, int tn);
void drawMesh(Mesh *mesh, unsigned int shader_program);

#endif