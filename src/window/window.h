#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct window
{
    GLFWwindow *glfw_window;
    int w, h;
    char title[200];
};

/**
 * @brief initialize the glfw window
 * 
 * @param window_w window width
 * @param window_h window height
 * @param title window title
 * @param fn the GetProcAdress function needed by OpenGL
 * @return struct window 
 */
struct window initWindow(int window_w, int window_h, char *title, void **fn);

void processInput(GLFWwindow *window);

#endif