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

    initOnlyCameraUniform(outline);
    
    struct model 
    model = 
        loadModel("/home/uhrbaan/Documents/code/com.learnopengl/res/models/self-made/small-scene/scene.obj",
                  aiProcess_Triangulate | aiProcess_FlipUVs),
    scaled_model = 
        loadModel("/home/uhrbaan/Documents/code/com.learnopengl/res/models/self-made/small-scene/scene_scalded.obj",
                  aiProcess_Triangulate | aiProcess_FlipUVs),
    wood = 
        loadModel("/home/uhrbaan/Documents/code/com.learnopengl/res/models/self-made/firstmodelwblender/untitled.obj",
                  aiProcess_Triangulate | aiProcess_FlipUVs);

    createDirectionalLight(0,
        (vec3){-0.2f, -1.0f, -0.3f}, (vec3){ 0.2f,  0.2f,  0.2f},
        (vec3){ 0.7f,  0.7f,  0.6f}, (vec3){ 0.7f,  0.7f,  0.6f});
    updateDirectionalLight(0, sp);

    createPointLight(0,
        (vec3){-7.7f, 6.0f,-3.6f}, (vec3){ 0.0f, 0.0f, 0.0f},
        (vec3){ 1.0f, 0.5f, 0.5f}, (vec3){ 1.0f, 0.5f, 0.5f},
        0.09f, 0.032f);
    updatePointLight(0, sp);

    createSpotLight(0, 
        (vec3){ -.2f, 3.5f, -.5f}, (vec3){ 0.0f, 0.0f, -1.f},
        (vec3){ 0.0f, 0.0f, 0.0f}, (vec3){ 1.0f, 1.0f, 1.0f},
        (vec3){ 1.0f, 1.0f, 1.0f}, 0.045f, 0.0075f, 20.0f, 25.0f);
    updateSpotLight(0, sp);

    // global opengl state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    while(!glfwWindowShouldClose(state.window.glfw_window))
    {
        float current_frame = glfwGetTime();
        state.delta_time    = current_frame - last_frame;
        last_frame          = current_frame;

        glfwPollEvents();
        movement(state.window.glfw_window);

        // updateCamera(&state.camera, sp);
        // glm_vec3_copy(state.camera.pos, spot_light[0].position);
        // glm_vec3_copy(state.camera.z,   spot_light[0].direction);
        // updateSpotLight(0, sp);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | 
                GL_DEPTH_BUFFER_BIT | 
                GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00); // dont update stencil buffer
        glm_mat4_identity(state.camera.model);
        updateCamera(&state.camera, sp);
        drawModel(model, sp);

        glStencilFunc(GL_ALWAYS, 1, 0xff); // update stencil mask with what is drawn
        glStencilMask(0xff); // write to stencil buffer
        glm_translate(state.camera.model, (vec3){-5.0f, 1.2f, -2.1f});
        glm_scale(state.camera.model, (vec3){3.f, 4.f, 3.f});
        updateCamera(&state.camera, sp);
        drawModel(wood, sp);

        glStencilFunc(GL_NOTEQUAL, 1, 0xff); // draw what is not equal to 1 in the buffer
        glStencilMask(0x00); // do not update the buffer
        glDisable(GL_DEPTH_TEST); // disable buffer depth testing
        // glm_translate(state.camera.model, (vec3){-2.0f, 0.f, 0.f});
        glm_scale(state.camera.model, (vec3){1.01f, 1.01f, 1.01f});
        updateCamera(&state.camera, outline);
        drawModel(wood, outline);

        glStencilMask(0xff); // reset stencil buffer
        glStencilFunc(GL_ALWAYS, 1, 0xff);

        glfwSwapBuffers(state.window.glfw_window);
        msleep(16);
    }
    glfwTerminate();
    return 0; 
}

// TODO stencil buffer

// TODO load 2 models at the same time on screen

// T*R*S