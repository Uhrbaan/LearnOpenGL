#ifndef OPENGL_H
#define OPENGL_H

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

enum texture_type { // based on assimps list
    none,
    diffuse,
    specular,
    ambient,
    emissive,
    normals,
    shininess,
    opacity,
    displacement,
    lightmap,
    reflection,
    base_color,
    normal_camera,
    emission_color,
    metalic,
    diffuse_roughness,
    ambient_occlusion,
    sheen,
    clear_coat,
    transmission,
    unknown
};
struct datawloc
{
    void *data;
    unsigned int uniform_location, shader_program;
};

int initGlad(int x, int y, int w, int h, void* fn_proc_adress);

extern unsigned int loadShaderProgram(const char *vs_path, const char *fs_path);

unsigned int loadGLTexture(const char *path); 

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

#endif