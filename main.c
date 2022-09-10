#include <stdio.h>
#include <assimp/cimport.h>
#include "src/global.h"
#include "src/render/shading.h"
#include "src/game/input/input.h"

float last_frame = 0;

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initGlobalState( 800, 600, "testing..." );
    unsigned int sp = 0, outline = 0;
    sp = loadShaderProgram("./res/glsl/vs.vs", "./res/glsl/light.fs");
    outline = loadShaderProgram("./res/glsl/vs.vs", "./res/glsl/outline.fs");
    initShading(sp);
    state.camera = 
        initCamera(false, 800, 600,
                   (vec3){ 0.2f,  1.8f,  5.3f}, 
                   (vec3){ 1.0f,  0.0f,  0.0f}, 
                   (vec3){ 0.0f,  1.0f,  0.0f}, 
                   (vec3){ 0.01f, 0.2f,  1.0f},
                   glm_rad(45.0f), glm_rad(25.f), glm_rad(80.f), glm_rad(1.f), 
                   sp);
    
    struct model model = {0};
    model = loadModel("/home/uhrbaan/Documents/code/com.learnopengl/res/models/self-made/small-scene/scene.obj",
                  aiProcess_Triangulate | aiProcess_FlipUVs);

    glm_vec3_copy((vec3){-0.2f, -1.0f, -0.3f}, directional_light[0].direction); // directional light source
    glm_vec3_copy((vec3){ 0.8f,  0.8f,  0.8f}, directional_light[0].ambient);
    glm_vec3_copy((vec3){ 1.0f,  1.0f,  1.0f}, directional_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.5f,  0.5f,  0.5f}, directional_light[0].specular);
    updateDirectionalLight(0, sp);

    glfwSetInputMode(state.window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while(!glfwWindowShouldClose(state.window.glfw_window))
    {
        float current_frame = glfwGetTime();
        state.delta_time    = current_frame - last_frame;
        last_frame          = current_frame;

        glfwPollEvents();
        movement(state.window.glfw_window);

        updateCamera(&state.camera, sp);
        glm_vec3_copy(state.camera.pos, spot_light[0].position);
        glm_vec3_copy(state.camera.z,   spot_light[0].direction);
        updatespotLight(0, sp);

        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // clearing color & depth buffer

        // stencil buffer test


        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        drawModel(model, sp);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        drawModel(model, outline);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(state.window.glfw_window);
        msleep(16);
    }
    glfwTerminate();
    return 0; 
}

/**
 * @brief notes
 * 
 * local space > world space > view space > clip space > screen space
 * 
 */

// TODO handle multiple shaders
// will be difficult *sad*