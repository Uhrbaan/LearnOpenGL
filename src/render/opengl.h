#ifndef OPENGL_H
#define OPENGL_H

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

struct datawloc
{
    void *data;
    unsigned int uniform_location, shader_program;
    void (*update_fn_vec)(int, int, void*);
    void (*update_fn_mat)(int, int, unsigned char, void*);
};

struct datawloc createDatawLoc(void *p, const char *name, 
                               unsigned int shader_program,
                               void *update_fn_vec, void *update_fn_mat);
void updateUniform(struct datawloc d);

int initGlad(int x, int y, int w, int h, void* fn_proc_adress);
// do error checking
unsigned int loadShader(const char *path, unsigned int type);
unsigned int createShaderProgram(unsigned int vs, unsigned int fs);
// dont forget to free data after
unsigned int create2dTexure(unsigned char *data, int channels, int w, int h);

unsigned int loadTexture(const char *path);
void useTexture(int index, char *uniform_name, unsigned int shader_program, 
                unsigned int gl_id);
void drawElements(unsigned int shader_program, unsigned int vao, int n_indices);
/**
 * @brief generate vao, vbo & ebo from data
 * 
 * @param vbo generate vbo (optional)
 * @param ebo generate ebo (optional)
 * @param vertices data of the pos | norm | uv
 * @param vert_sz size of the vertices
 * @param indices data of the indices
 * @param indi_sz size of indices
 * @param stride stride
 * @return generated vao 
 */
unsigned int genVao(unsigned int *vbo, unsigned int *ebo, 
                    void *vertices, size_t vert_sz, 
                    void *indices, size_t indi_sz, size_t stride);

// returns the number of the texture
unsigned int bindTextureAdd(unsigned int loc, int texture_type, 
                            unsigned int id, unsigned int shader_program);
void bindTextureReset();

#endif