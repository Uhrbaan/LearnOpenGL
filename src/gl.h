#ifndef gl_h
#define gl_h

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

#endif