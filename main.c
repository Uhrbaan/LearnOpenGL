/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * lighting
 *  - ambiant
 *  - diffuse -> simulates where light comes from
 *  - specular -> simulates bright spots on objects
 *  -> combined (Phong)
 *  - directional light(16.1)
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
    
    unsigned int diffuse_map, specular_map, emission_map;
    diffuse_map = 
        file2texture("res/textures/container2.png", GL_RGBA, GL_TEXTURE_2D);
    specular_map = 
        file2texture("res/textures/container2_specular.png", GL_RGBA, GL_TEXTURE_2D);
    emission_map = 
        file2texture("res/textures/matrix.jpg", GL_RGB, GL_TEXTURE_2D);
// material
    Material material = {
        diffuse_map,        // diffuse
        specular_map,       // specular
        emission_map,       // emission
        32.0f,              // shininess
        light_illuminated   // shader_program
    };
    updateMaterial(material);
    Light light = {
        {0.3f, 0.5f, 1.4f}, // pos
        {0.1f, 0.1f, 0.1f}, // ambiant
        {0.8f, 0.8f, 0.8f}, // diffuse
        {1.0f, 1.0f, 1.0f}, // specular
        1.0f, 0.09f, 0.032f,// attenuation
        light_illuminated   // shader_program
    };
    updateLight(light);
    setUniform(light_illuminated, GL_FLOAT_VEC3, "light.direction", -0.2f, -1.0f, -0.3f);

// models
    float vertices[] = MODEL_CUBE_NORMAL_TEXTURE;
    vec3 cube_pos[] = MODEL_10_CUBES_POS;
    unsigned int vbo;
    vbo = genBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    model light_cube, cube, box;
    light_cube = createModel(
        (vec3){light.position[0], light.position[1], light.position[2]},
        (vec3){0.2f, 0.2f, 0.2f}, 
        createUniformMatrix("model", 1, &light_src),
        genVAO(vbo, 0, 1, 
               0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0)
        );

    cube = createModel(
        (vec3){0.0f, 0.0f, 0.0f}, 
        (vec3){1.0f, 1.0f, 1.0f}, 
        createUniformMatrix("model", 1,&light_illuminated),
        genVAO(vbo, 0, 3,
               0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0,
               1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)),
               2, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(6*sizeof(float)))
        );

    box = createModel(
        (vec3){0.0f, 0.0f, 4.0f},
        (vec3){1.2f, 1.2f, 1.2f},
        createUniformMatrix("model", 1, &light_illuminated),
        genVAO(vbo, 0, 1,
               0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0,
               1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)))
        );

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
        glUniform3fv(cam_pos_loc, 1, cam.pos);

        // rendering
        // clear screen
        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // making light circle around cube
        // glm_vec3_copy((vec3){sin(current_frame)*4, cos(current_frame)*4, sin(current_frame)*4}, light_cube.pos);
        // glm_mat4_identity(light_cube.transform.m);
        // glm_translate(light_cube.transform.m, light_cube.pos);
        // // update light source position
        // memcpy(light.position, light_cube.pos, sizeof(vec3));
        // updateLight(light);
        // renderModel(light_cube, light_src, 0, 36); // light represented by white cube

        setUniform(light_illuminated, GL_FLOAT_VEC3, "light.position", cam.pos[0], cam.pos[1], cam.pos[2]);
        setUniform(light_illuminated, GL_FLOAT_VEC3, "light.direction", cam.z[0], cam.z[1], cam.z[2]);
        setUniform(light_illuminated, GL_FLOAT, "light.cutoff", cos(glm_rad(12.5f)));
        setUniform(light_illuminated, GL_FLOAT, "light.outer_cutoff", cos(glm_rad(18.0f)));

        // render multiple objects
        for(unsigned int i = 0; i < 10; i++)
        {
            glm_mat4_identity(cube.transform.m);
            glm_translate(cube.transform.m, cube_pos[i]);
            float angle = 20.0f * i;
            glm_rotate(cube.transform.m, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.diffuse_map);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material.specular_map);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, material.emission_map);
            renderModel(cube, light_illuminated, 0, 36);
        }


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