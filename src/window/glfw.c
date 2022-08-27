#include "glfw.h"
#include "../utils/utils.h"
#include <cglm/cglm.h>

float last_frame;
int initGLFW(GLFWwindow **window, int width, int height, 
                    const char* title, GLFWmonitor* monitor, 
                    GLFWwindow* share,
    void (*scroll_callback)(GLFWwindow* window, double xoffset, double yoffset),
    void (*mouse_callback)(GLFWwindow* window, double x, double y),
    void (*framebuffer_size_callback)(GLFWwindow* window, int width, int height))
{
    if (glfwInit() == GLFW_TRUE)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        *window = glfwCreateWindow(width, height, title, monitor, share);
        if (*window == NULL)
        {
            fprintf(stderr, TERM_COL_ERROR("error:") 
                    "failded to create glfw window");
            glfwTerminate();
            return 1;
        }

        // touts les callbacks
        glfwMakeContextCurrent(*window);
        // quoi faire en quand la fenêtre change de taille
        glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
        // actions de la souris
        glfwSetCursorPosCallback(*window, mouse_callback);
        // scroll
        glfwSetScrollCallback(*window, scroll_callback);
        
        return 0;
    }
    return 1;
}