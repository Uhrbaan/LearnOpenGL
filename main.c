/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
 */

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

        // 
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    glfwTerminate();

    return 0;
}
