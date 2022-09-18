#ifndef OPENGL_H
#define OPENGL_H

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

extern unsigned int SHADER_PROGRAM_N;

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

int initOpenGL(int x, int y, int w, int h, void* getProcAdress_fn);

extern unsigned int loadShaderProgram(const char *vs_path, const char *fs_path);
unsigned int createShaderProgram(unsigned int vs, unsigned int fs);
unsigned int loadShader(const char *path, unsigned int type);

unsigned int loadGLTexture(const char *path); 

void drawElements(unsigned int shader_program, unsigned int vao, int n_indices);
/**
 * @brief generate a vertex array object
 * 
 * @param vertices array of floats. the layout should be 3 floats for position, 
 *                 3 floats for the normal, 2 floats for the uv mapping
 * @param vetices_n total number of vertices
 * @param indices array of unsigned integers
 * @param indices_n number of indices
 * @param vbo_out reference to the vertex buffer object
 * @param ebo_out reference to the element buffer object
 * @return unsigned int 
 */
unsigned int generateVao(float        *vertices, int vertices_n,
                         unsigned int *indices,  int indices_n,
                         unsigned int *vbo_out, unsigned int *ebo_out);

#endif

// TODO support colors included in model without a texture
// TODO support different parameters depending on shader
// TODO customize rendering depending on shader