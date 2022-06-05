#ifndef model_h
#define model_h

#include <cglm/cglm.h>
#include "gl.h"

typedef struct model 
{
    vec3 pos, scale, rot;
    mat4wloc transform;
    unsigned int vao, texture;
    // render info
    int offset; unsigned int vertices;
} model;

unsigned int genBuffer(GLenum buffer_type, size_t data_size, void* data, 
                       GLenum usage);

unsigned int genVAO(unsigned int vbo, 
                    unsigned int ebo, 
                    unsigned int shader_loc_index, 
                    unsigned int size, 
                    GLenum data_type, 
                    bool normalize, 
                    size_t stride, 
                    void* offset);

model createModel(vec3 pos, vec3 scale, vec3 rot, mat4wloc transform, 
                  unsigned int vao, unsigned int texture);

void renderModel(model model, unsigned int shader_program, int offset, 
                 unsigned int nvertices);

#endif