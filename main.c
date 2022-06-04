/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * lighting
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

    unsigned int shader_light, shader_cube;
    if (!(shader_light = createShaderProgram("res/glsl/cube.vs", 
                                             "res/glsl/light.fs")))
        return 1;
    if (!(shader_cube  = createShaderProgram("res/glsl/cube.vs",
                                             "res/glsl/cube.fs")))
        return 1;

    // projection init
    unsigned int programs[] = {shader_light, shader_cube};
    mat4wloc l_transform = createUniformMatrix("u_transform", 1, &shader_light);
    mat4wloc c_transform = createUniformMatrix("u_transform", 1, &shader_cube);
    cam.view = createUniformMatrix("u_view", 2, programs);
    cam.projection = createUniformMatrix("u_projection", 2, programs);
    glm_perspective(glm_rad(45.0f), 1.3f, 0.1f, 100.0f, cam.projection.m);
    updateUniformMatrix(cam.projection, 0);
    
    float vertices[] = MODEL_CUBE;
    unsigned int vbo;
    model light = createModel((vec3){1.2f, 1.0f, 2.0f},
                              (vec3){0.2f, 0.2f, 0.2f}, (vec3){0},
                              &l_transform, 0, 0), 
          cube  = createModel((vec3){0.0f}, (vec3){0.0f}, (vec3){0.0f}, 
                              &c_transform, 0, 0);
    vbo = genBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    light.vao = genVAO(vbo, 0, 0, 3, GL_FLOAT, false, 3*sizeof(float), (void*)0);
    cube.vao  = genVAO(vbo, 0, 0, 3, GL_FLOAT, false, 3*sizeof(float), (void*)0);

    // light and cube pos&shape
    glm_mat4_identity(light.transform->m);
    glm_translate(light.transform->m, light.pos);
    glm_scale(light.transform->m, light.scale);

    // use color lighting
    glUseProgram(shader_light);
    setUniform(shader_light, GL_FLOAT_VEC3, "object_color", 1.0f, 0.5f, 0.31f);
    setUniform(shader_light, GL_FLOAT_VEC3, "light_color", 1.0f, 1.0f, 1.0f);

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

        glUseProgram(shader_light);
        glBindVertexArray(light.vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(shader_cube);
        glBindVertexArray(cube.vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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