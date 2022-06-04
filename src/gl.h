#ifndef gl_h
#define gl_h

#include <stdarg.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "utils/stb_image.h"

typedef struct uniform_mat4 
{
    mat4 m;
    unsigned int n_programs, *uniform_locations, *shader_programs;
    // the lists are to modify multiple shaders with the same uniform name at 
    // the same time
} mat4wloc;

// initialise glad (OpenGL), fait la taille du ViewPort (position x,y,w,h)
int initGLAD(GLint x, GLint y, GLsizei width, GLsizei height);

// crée un shader depuis un fichier GLSL
unsigned int file2shader(const char *file_path, GLenum shader_type);

// link les shader à shader_program, retourne 1 si erreur
unsigned int createShaderProgram(const char *vertex_shader, 
                                 const char *fragment_shader);

// set uniform (without bool, use int)
unsigned int setUniform(unsigned int shader_program, int type,
                        const char *name, ...);

// load texture
unsigned int file2texture(const char *img_path, GLenum color_format, 
                          GLenum texture_type);

// set texture param
void setTextureParam(int n, unsigned int texture, GLenum texture_type, ...);

mat4wloc createUniformMatrix(const char *uniform_name, unsigned int n_programs,
                             unsigned int *shader_programs);
void updateUniformMatrix(mat4wloc um4, int transpose);

#endif