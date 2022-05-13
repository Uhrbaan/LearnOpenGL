#ifndef gl_h
#define gl_h

#include <stdarg.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// retourne 1 si n'a pas fonctionné
int initGLFW(GLFWwindow **window, int width, int height,
             const char* title, GLFWmonitor* monitor,
             GLFWwindow* share);

// initialise glad (OpenGL), fait la taille du ViewPort (position x,y,w,h)
int initGLAD(GLint x, GLint y, GLsizei width, GLsizei height);

// fonction that processes all the inputs
void processInput(GLFWwindow *window);

// crée un shader depuis un fichier GLSL
unsigned int FILE2shader(const char *file_path, GLenum shader_type);

// crée un shader program à partir de n shaders
unsigned int linkShaders2program(int n, ...);

// set uniform (without bool, use int)
unsigned int setUniform(unsigned int shader_program, int type,
                        const char *name, ...);

// load texture + create mipmap
unsigned int FILE2texture(const char *img_path, GLenum texture_type);

#endif