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
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set entire window color
        glClear(GL_COLOR_BUFFER_BIT);
        // possible bit buffers to set:
        //  GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.

        // swapping buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
