#include <string.h>
#include "model.h"

unsigned int genBuffer(GLenum buffer_type, size_t data_size, void* data, GLenum usage)
{
    unsigned int buff;
    glGenBuffers(1, &buff);
    glBufferData(buffer_type, data_size, data, usage);
    return buff;
};

unsigned int genVAO(unsigned int vbo, 
                    unsigned int ebo, 
                    unsigned int shader_loc_index, 
                    unsigned int size, 
                    GLenum data_type, 
                    bool normalize, 
                    size_t stride, 
                    void* offset)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(shader_loc_index, size, data_type, normalize, stride, offset);
    glEnableVertexAttribArray(shader_loc_index);
    return vao;
}

model createModel(vec3 pos, vec3 scale, vec3 rot, mat4wloc *transform, 
                  unsigned int vao, unsigned int texture)
{
    model model = {0};
    memcpy(model.pos, pos, sizeof(vec3));
    memcpy(model.scale, scale, sizeof(vec3));
    memcpy(model.rot, rot, sizeof(vec3));
    model.transform = transform;
    model.vao = vao;
    model.texture = texture;
    return model;
}