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
                  aiProcess_Triangulate | aiProcess_FlipUVs),
    garden = 
        loadModel("/home/uhrbaan/Documents/code/com.learnopengl/res/models/self-made/garden/garden.obj",
                  aiProcess_Triangulate | aiProcess_FlipUVs);

    createDirectionalLight(0,
        (vec3){-0.2f, -1.0f, -0.3f}, (vec3){ 0.55f,  0.55f,  0.5f},
        (vec3){ 0.9f,  0.9f,  0.9f}, (vec3){ 1.0f,  1.0f,  1.0f});
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

    float grass_vert[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    vec3 grass_pos[] = {
        {-1.5f, 0.0f, -0.48f},
        { 1.5f, 0.0f,  0.51f},
        { 0.0f, 0.0f,  0.7f},
        {-0.3f, 0.0f, -2.3f},
        { 0.5f, 0.0f, -0.6f}
    };
    unsigned int grass_vao, grass_vbo, grass_text, grass_sp;
    glGenVertexArrays(1, &grass_vao);
    glGenBuffers(1, &grass_vbo);
    glBindVertexArray(grass_vao);
    glBindBuffer(GL_ARRAY_BUFFER, grass_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vert), grass_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float)*5, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float)*5, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    grass_text = loadGLTexture("/home/uhrbaan/Documents/code/com.learnopengl/res/textures/grass.png");
    grass_sp   = loadShaderProgram("res/glsl/grass.vs", "res/glsl/grass.fs");

    // global opengl state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
                GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00); // dont update stencil buffer
        
        glm_mat4_identity(state.camera.model);
        updateCamera(&state.camera, sp);
        drawModel(model, sp);

        glm_mat4_identity(state.camera.model);
        glm_translate(state.camera.model, (vec3){9.3f, 2.1f, 6.0f});
        updateCamera(&state.camera, sp);
        drawModel(garden, sp);

        glBindVertexArray(grass_vao);
        glBindTexture(GL_TEXTURE_2D, grass_text);
        for (int i=0; i<sizeof(grass_pos)/sizeof(vec3); i++)
        {
            glm_mat4_identity(state.camera.model);
            glm_translate(state.camera.model, grass_pos[i]);
            updateCamera(&state.camera, grass_sp);
            glUseProgram(grass_sp);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(grass_pos)/sizeof(vec3));
        }

        // glStencilFunc(GL_ALWAYS, 1, 0xff); // update stencil mask with what is drawn
        // glStencilMask(0xff); // write to stencil buffer

        // glm_mat4_identity(state.camera.model);
        // glm_translate(state.camera.model, (vec3){-5.0f, 1.2f, -2.1f});
        // glm_scale(state.camera.model, (vec3){3.f, 4.f, 3.f});
        // updateCamera(&state.camera, sp);
        // drawModel(wood, sp);

        // glStencilFunc(GL_NOTEQUAL, 1, 0xff); // draw what is not equal to 1 in the buffer
        // glStencilMask(0x00); // do not update the buffer
        // glDisable(GL_DEPTH_TEST); // disable buffer depth testing

        // // glm_translate(state.camera.model, (vec3){-2.0f, 0.f, 0.f});
        // glm_scale(state.camera.model, (vec3){1.01f, 1.01f, 1.01f});
        // updateCamera(&state.camera, outline);
        // drawModel(wood, outline);

        // glStencilMask(0xff); // reset stencil buffer
        // glStencilFunc(GL_ALWAYS, 1, 0xff);

        glfwSwapBuffers(state.window.glfw_window);
        msleep(16);
    }
    glfwTerminate();
    return 0; 
}

// TODO stencil buffer
// FIXME corrupted size vs. prev_size shows when quitting
// TODO load 2 models at the same time on screen

// T*R*S