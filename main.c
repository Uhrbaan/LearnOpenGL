/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * lighting
 *  - ambiant
 *  - diffuse -> simulates where light comes from
 *  - specular -> simulates bright spots on objects
 *  -> combined (Phong)
 * 
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

    // shaders
    unsigned int light_src, light_illuminated;
    if (!(light_src = createShaderProgram("res/glsl/cube.vs", 
                                          "res/glsl/cube.fs")))
        return 1;
    if (!(light_illuminated  = createShaderProgram("res/glsl/light.vs",
                                                   "res/glsl/light.fs")))
        return 1;

    // projection init
    unsigned int programs[] = {light_src, light_illuminated};
    cam.view = createUniformMatrix("view", 2, programs);
    cam.projection = createUniformMatrix("projection", 2, programs);
    glm_perspective(glm_rad(45.0f), 1.3f, 0.1f, 100.0f, cam.projection.m);
    updateUniformMatrix(cam.projection, 0);
    
    // material
    Material material = {
        {0.5f, 0.2f, 0.2f},// ambiant
        {1.0f, 0.3f, 0.3f},// diffuse
        {1.0f, 0.5f, 0.5f},// specular
        64.0f,              // shininess
        light_illuminated   // shader_program
    };
    updateMaterial(material);
    Light light = {
        {0.2f, 0.5f, 1.2f}, // pos
        {0.5f, 0.5f, 0.5f}, // ambiant
        {1.0f, 1.0f, 1.0f}, // diffuse
        {1.0f, 1.0f, 1.0f}, // specular
        light_illuminated   // shader_program
    };
    updateLight(light);

    // models
    float vertices[] = MODEL_CUBE_NORMAL_TEXTURE;
    unsigned int vbo;
    vbo = genBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    model light_cube = createModel((vec3){light.position[0], light.position[1], light.position[2]},
                                   (vec3){0.2f, 0.2f, 0.2f}, 
                                   createUniformMatrix("model", 1, 
                                                       &light_src),
                                   genVAO(vbo, 0, 0, 3, GL_FLOAT, false, 
                                           6*sizeof(float), (void*)0),
                                   0),
          cube  = createModel((vec3){0.0f, 0.0f, 0.0f}, 
                              (vec3){1.0f, 1.0f, 1.0f}, 
                              createUniformMatrix("model", 1, 
                                                  &light_illuminated),
                              genVAO(vbo, 0, 0, 3, GL_FLOAT, false, 
                                     6*sizeof(float), (void*)0),
                              0);
    // set normals to cube sides
    glBindVertexArray(cube.vao);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexArrayAttrib(cube.vao, 1);

    // send camera pos
    unsigned int cam_pos_loc;
    cam_pos_loc = glGetUniformLocation(light_illuminated, "cam_pos");

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

        
        // making light circle around cube
        glm_vec3_copy((vec3){sin(current_frame)*4, cos(current_frame)*4, sin(current_frame)*4}, light_cube.pos);
        glm_mat4_identity(light_cube.transform.m);
        glm_translate(light_cube.transform.m, light_cube.pos);
        // update light source position
        memcpy(light.position, light_cube.pos, sizeof(vec3));
        updateLight(light);
        glUniform3fv(cam_pos_loc, 1, cam.pos);

        renderModel(light_cube, light_src, 0, 36); // light represented by white cube
        renderModel(cube, light_illuminated, 0, 36);

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