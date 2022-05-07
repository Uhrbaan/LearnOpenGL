#include <stdio.h>

#include "gl.h"
#include "utils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int initGLFW(GLFWwindow **window, int width, int height, 
             const char* title, GLFWmonitor* monitor, 
             GLFWwindow* share)
{
    if (likely(glfwInit() == GLFW_TRUE))
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        *window = glfwCreateWindow(width, height, title, monitor, share);
        if (unlikely(*window == NULL))
        {
            printf(TERM_COL_ERROR("error") ":Failed to create GLFW window\n");
            glfwTerminate();
            return 1;
        }
        glfwMakeContextCurrent(*window);
        glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
            // quoi faire en quand la fenêtre change de taille
        
        return 0;
    }
    return 1;
}

int initGLAD(GLint x, GLint y, GLsizei width, GLsizei height)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf(TERM_COL_ERROR("error") ": Failed to initialize GLAD\n");
        return 1;
    }
    glViewport(x, y, width, height);
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // callback function that tells what to do on a resize
    glViewport(0, 0, width, height);
}