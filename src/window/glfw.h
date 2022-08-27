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
 */
int initGLFW(GLFWwindow **window, int width, int height, 
                    const char* title, GLFWmonitor* monitor, 
                    GLFWwindow* share,
                    void (*mouse_callback)
                        (GLFWwindow* window, double x, double y),
                    void (*scroll_callback)
                        (GLFWwindow* window, double xoffset, double yoffset),
                    void (*framebuffer_size_callback)
                        (GLFWwindow* window, int width, int height));

void processInput(GLFWwindow *window);

#endif