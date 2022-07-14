#include <stdio.h>

#include "src/graphic.h"
#include "src/global.h"
#include "src/utils/utils.h"

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initApplicationGlobalState(
        800, 600, "testing...",
        false, (vec3){0.0f, 0.0f, 0.0f}, 
               (vec3){1.0f, 0.0f, 0.0f}, 
               (vec3){0.0f, 1.0f, 0.0f}, 
               (vec3){0.0f, 0.0f,-1.0f},
        45.0f, 0.0f, 0.0f, 0.0f
    );

    struct model m =
        loadModel("res/models/backpack/backpack.obj");
    for (int i=0; i<m.n_meshes; i++)
        generateMeshVAO(&m.meshes[i]); 

    unsigned int sp = 
        createShaderProgram(
            loadShader("res/glsl/vs.vs", GL_VERTEX_SHADER),
            loadShader("res/glsl/fs.fs", GL_FRAGMENT_SHADER));

    while (!glfwWindowShouldClose(state.window.glfw_window))
    {
        drawModel(m, sp);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(255.0f, 255.0f, 100.0f, 255.0f);
        glfwSwapBuffers(state.window.glfw_window);
        msleep(100);
    }

    return 0;
} 