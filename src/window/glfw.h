#ifndef GLFW_H
#define GLFW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct window
{
    GLFWwindow *glfw_window;
    int w, h;
    char title[200];
};

/**
 * @brief initialises the glfw window
 * @return 1 if something went wrong, 0 if alright
 * TODO support more glfw callbacks
 */
void * initGLFW(GLFWwindow **window, int width, int height, 
                const char* title, GLFWmonitor* monitor, GLFWwindow* share);

void processInput(GLFWwindow *window);

#endif