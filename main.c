#include <stdio.h>

#include "src/graphic.h"
#include "src/global.h"
#include "src/utils/utils.h"

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initApplicationGlobalState(
        800, 600, "testing...",
        false, (vec3){0.0f, 0.0f, 1000.0f}, 
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
    
    struct datawloc spaces[3] = {};
    spaces[0] = createDatawLoc(&state.camera.model, "model", sp, NULL, (void*)glUniformMatrix4fv);
    spaces[1] = createDatawLoc(&state.camera.view, "view", sp, NULL, (void*)glUniformMatrix4fv);
    spaces[2] = createDatawLoc(&state.camera.projection, "projection", sp, NULL, (void*)glUniformMatrix4fv);

    while (!glfwWindowShouldClose(state.window.glfw_window))
    {
        processInput(state.window.glfw_window);

        updateCamera(&state.camera);
        for (int i=0; i<3; i++)
            updateUniform(spaces[i]);

        glClearColor(85.0f, 26.0f, 139.0f, 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawModel(m, sp);
        
        glfwSwapBuffers(state.window.glfw_window);
        glfwWaitEvents();
    }

    return 0;
} 