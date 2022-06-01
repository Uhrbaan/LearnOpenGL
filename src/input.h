#ifndef input_h
#define input_h

#include "gl.h"

// retourne 1 si n'a pas fonctionné
int initGLFW(GLFWwindow **window, int width, int height,
             const char* title, GLFWmonitor* monitor,
             GLFWwindow* share);

// fonction that processes all the inputs
void processInput(GLFWwindow *window);

#endif