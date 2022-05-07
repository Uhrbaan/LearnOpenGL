#include <stdio.h>
#include <stdlib.h>

#include "src/gl.h"
#include "src/utils.h"

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, 800, 600, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, 800, 600);

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
