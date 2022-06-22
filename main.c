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
    Directional_light dir_light = 
    {
        {-0.2f, -1.0f, -0.3f},
        { 0.2f,  0.2f,  0.2f},
        { 0.8f,  0.8f,  0.8f},
        { 1.0f,  1.0f,  1.0f},
        light_illuminated    
    };
    Point_light pt_lights[4] = 
    {{  {0.7f, 0.2f, 2.0f},
        {0.2f, 0.2f, 0.2f},
        {1.0f, 0.0f, 0.0f},
        {0.9f, 0.9f, 0.9f},
        1.0f, 0.35f, 0.44f, 
        light_illuminated   },
     {  {2.3f, -3.3f, -4.0f},
        {0.2f, 0.2f, 0.2f},
        {0.0f, 1.0f, 0.8f},
        {0.8f, 1.0f, 0.9f},
        1.0f, 0.09f, 0.032f,
        light_illuminated   },
     {  {-4.0f, 2.0f, -12.0f},
        {0.5f, 0.5f, 0.3f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        1.0f, 0.07f, 0.017f,
        light_illuminated   },
     {  {0.0f, 0.0f, -3.0f},
        {0.3f, 0.1f, 0.1f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.7f, 0.7f},
        1.0f, 0.22f, 0.20f,
        light_illuminated   }
    };
    Spot_light spotlight = {
        {cam.pos[0], cam.pos[1], cam.pos[2]},
        {cam.z[0], cam.z[1], cam.z[2]},
        {0.2f, 0.2f, 0.2f},
        {0.8f, 0.8f, 0.8f},
        {1.0f, 1.0f, 1.0f},
        1.0f, 0.09f, 0.032f,
        cos(glm_rad(12.5f)), cos(glm_rad(17.5f)),
        light_illuminated
    };

    updateLight(&dir_light, "dir_light", DIRECTIONAL);
    updateLight(&pt_lights[0], "pt_lights[0]", POINT);
    updateLight(&pt_lights[1], "pt_lights[1]", POINT);
    updateLight(&pt_lights[2], "pt_lights[2]", POINT);
    updateLight(&pt_lights[3], "pt_lights[3]", POINT);
    updateLight(&spotlight, "spotlight", SPOT);

// models -> loaded w/ assimp
    Model model = {0};
    loadModel(&model, "/home/uhrbaan/Documents/Code/C/openGL/com.learnopengl/res/models/backpack/backpack.obj");

    // uniform locations
    unsigned int cam_pos_loc, light_cube_col;
    cam_pos_loc = glGetUniformLocation(light_illuminated, "cam_pos");
    light_cube_col = glGetUniformLocation(light_src, "light_color");

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

        drawModel(&model, light_illuminated);

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