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
    // outline = loadShaderProgram("./res/glsl/vs.vs", "./res/glsl/outline.fs");
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

    glm_vec3_copy((vec3){-0.2f, -1.0f, -0.3f}, directional_light[0].direction);
    glm_vec3_copy((vec3){ 0.2f,  0.2f,  0.2f}, directional_light[0].ambient);
    glm_vec3_copy((vec3){ 0.7f,  0.7f,  0.6f}, directional_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.7f,  0.7f,  0.6f}, directional_light[0].specular);
    updateDirectionalLight(0, sp);

    glm_vec3_copy((vec3){-7.7f, 6.0f,-3.6f}, point_light[0].position);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f}, point_light[0].ambient);
    glm_vec3_copy((vec3){ 1.0f, 0.5f, 0.5f}, point_light[0].diffuse);
    glm_vec3_copy((vec3){ 1.0f, 0.5f, 0.5f}, point_light[0].specular);
    point_light[0].constant  = 1.0f;
    point_light[0].linear    = 0.09f;
    point_light[0].quadratic = 0.032f;
    updatePointLight(0, sp);

    glm_vec3_copy((vec3){ -.2f, 3.5f, -.5f}, spot_light[0].position);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, -1.f}, spot_light[0].direction);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f}, spot_light[0].ambient);
    glm_vec3_copy((vec3){ 1.0f, 1.0f, 1.0f}, spot_light[0].diffuse);
    glm_vec3_copy((vec3){ 1.0f, 1.0f, 1.0f}, spot_light[0].specular);
    spot_light[0].constant  = 1.0f;
    spot_light[0].linear    = 0.045f;
    spot_light[0].quadratic = 0.0075f;
    spot_light[0].cutoff       = cos(glm_rad(20.f));
    spot_light[0].outer_cutoff = cos(glm_rad(35.f));
    updatespotLight(0, sp);

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

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        drawModel(model, sp);


        glfwSwapBuffers(state.window.glfw_window);
        msleep(16);
    }
    glfwTerminate();
    return 0; 
}

// TODO handle multiple shaders
// will be difficult *sad*