/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * Loading models into game
 * -> Assimp loading library
 * 
 * 
 */

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <cglm/cglm.h>

#include "src/gl.h"
#include "src/utils/utils.h"
#include "src/input.h"
#include "src/camera.h"
#include "src/model.h"
#include "src/lighting.h"

const float SCR_W = 800.0f;
const float SCR_H = 600.0f;

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, SCR_W, SCR_H, "Learn Opengl", NULL, NULL))
        return 1;
    initGLAD(0, 0, SCR_W, SCR_H);
    cam = initCamera((vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 0.0f, -1.0f}, 
                     (vec3){0.0f, 1.0f, 0.0f}, SCR_W, SCR_H, 1.0f);

// shader
    unsigned int sp;
    if (!(sp = createShaderProgram("res/glsl/vs.vs", "res/glsl/fs.fs")))
        return 1;

    // projection init
    cam.view = createUniformMatrix("view", 1, &sp);
    cam.projection = createUniformMatrix("projection", 1, &sp);
    glm_perspective(glm_rad(45.0f), 1.3f, 0.1f, 100.0f, cam.projection.m);
    updateUniformMatrix(cam.projection, 0);
    
// models -> loaded w/ assimp
    Model model = {0};
    
    loadModel(&model, "res/models/backpack/backpack.obj");
    
    float f[] = {
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
    unsigned int i[] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof f, f, GL_STATIC_DRAW);

    glBindVertexArray(ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof i, i, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof f, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof f, (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof f, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // TODO test if normal rendering works (should)
    // TODO understand why not rendering
    // TODO understand why funny numbers produced by assimp (consistently same) 
    // allows z-buffer testing-> discard behind
    glEnable(GL_DEPTH_TEST);
    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while(!glfwWindowShouldClose(window))
    {
        // uptdate delta
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawModel(&model, sp);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, i);

        // process
        updateCamera();
        updateUniformMatrix(cam.view, 0);

        // render
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }
    glfwTerminate();
    return 0;
}