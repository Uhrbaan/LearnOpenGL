/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * Camera
 *  simulating a camera
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

const float SCR_W = 800.0f;
const float SCR_H = 600.0f;

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, SCR_W, SCR_H, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, SCR_W, SCR_H);

    // shaders
    unsigned int shader_program;
    shader_program = glCreateProgram();
    if (linkShaders(shader_program, 2, 
                    FILE2shader("res/GLshaders.glsl/shader.vs",
                                GL_VERTEX_SHADER), 
                    FILE2shader("res/GLshaders.glsl/shader.fs", 
                                GL_FRAGMENT_SHADER)))
        return 1;

    initCamera((vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 0.0f, 0.0f}, 
               (vec3){0.0f, 1.0f, 0.0f}, SCR_W, SCR_H, 0.5f);

    // projection init
    mat4wloc model = createUniformMatrix("u_model", shader_program);
    cam.view = createUniformMatrix("u_view", shader_program);
    cam.projection = createUniformMatrix("u_projection", shader_program);
    glm_perspective(glm_rad(45.0f), 1.3f, 0.1f, 100.0f, cam.projection.m);
    updateUniformMatrix(cam.projection, 0);

    // camera
    memcpy(cam.pos, (vec3){0.0f, 0.0f, 3.0f}, sizeof(vec3));
    memcpy(cam.z, (vec3){0.0f, 0.0f, -1.0f}, sizeof(vec3));
    memcpy(cam.y, (vec3){0.0f, 1.0f, 0.0f}, sizeof(vec3));

    // model
    unsigned int texture;
    texture = FILE2texture("res/textures/safe_landing.jpg", GL_RGB, GL_TEXTURE_2D);
    

    float vertices[] = MODEL_CUBE; vec3 cubes_pos[] = MODEL_10_CUBES_POS;
    unsigned int vao, vbo;
    size_t stride, off_pos, off_tex;
    stride  = 5*sizeof(float);
    off_pos = 0;
    off_tex = 3*sizeof(float);
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)off_pos);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, stride, (void*)off_tex);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST); // allows testing for z-bufer
    // met la souris au centre et l'empêche de sortir
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

        glUseProgram(shader_program);
        glBindTexture(GL_TEXTURE_2D, texture);

        // process
        updateCamera();

        // render
        glBindVertexArray(vao);
        for (int i=0; i<10; i++)
        {
            glm_mat4_identity(model.m);
            glm_translate(model.m, cubes_pos[i]);
            float angle = 20.0f * i;
            glm_rotate(model.m, glm_rad(angle)*current_frame, (vec3){1.0f, 0.3f, 0.5f});
            updateUniformMatrix(model, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    // dealocating
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
 
    glfwTerminate();

    return 0;
}

/* fn help
 * glVertexAttribPointer(attribute position, 
 *                       number of elements, 
 *                       element type, 
 *                       normalize, 
 *                       stride,
 *                       offset) 
 */