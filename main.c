/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 * Coordinates system:
 *  - local space   -> coordinates relative to objects origin
 *      model matrix
 *  - world space   -> coordinates relative to the world
 *      view matrix
 *  - View space    -> coo relative to the camera's point of view
 *      projection matrix
 *  - Clip space    -> transforms to 0.0-1.0 coo & determins if on screen
 *      viewport transform
 *  - screen space  -> coo on the screen
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <cglm/cglm.h>

#include "src/gl.h"
#include "src/utils/utils.h"
#include "src/utils/matrix.h"

const float SCR_W = 800.0f;
const float SCR_H = 600.0f;

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, SCR_W, SCR_H, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, SCR_W, SCR_H);

    // <shaders>
    unsigned int shader_program;
    shader_program = glCreateProgram();
    if (linkShaders(shader_program, 2, 
                    FILE2shader("res/GLshaders.glsl/shader.vs",
                                GL_VERTEX_SHADER), 
                    FILE2shader("res/GLshaders.glsl/shader.fs", 
                                GL_FRAGMENT_SHADER)))
        return 1;
    
    /* projection
     * orthographic projection
     *  glm_ortho(left coo of the frustum,
     *            right,
     *            bottom,
     *            top,
     *            distance to near plane,
     *            far plane)
     * perspective projection
     *  glm_perspective(fov (in RAD, natural ~45°),
     *                  aspect ratio (vp_width/vp_height), 
     *                  distance to near plane, 
     *                  far plane)
     * -> V_clip = M_projection · M_view · M_model · V_local
     *  ! matrix multiplication reads from right to left
     */
    
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

    unsigned int model_loc, view_loc, projection_loc;
    model_loc = glGetUniformLocation(shader_program, "u_model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)model);
    view_loc = glGetUniformLocation(shader_program, "u_view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);
    projection_loc = glGetUniformLocation(shader_program, "u_projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float*)projection);

    float vertices[] = {
        // pos                // tex coo
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    vec3 cube_pos[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

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

    unsigned int texture;
    texture = FILE2texture("res/textures/safe_landing.jpg", GL_RGB, GL_TEXTURE_2D);
    glUseProgram(shader_program);
    glm_perspective(glm_rad(45.0f), SCR_W/SCR_H, 0.1f, 100.0f, projection);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float*)projection);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); // allows testing for z-bufer
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindTexture(GL_TEXTURE_2D, texture);

        // process
        glm_mat4_identity(view);
        glm_translate(view, (vec3){/* sin(glfwGetTime()) */0.0f, /* cos(glfwGetTime()) */0.0f, -3.0f});
        glm_rotate(view, glm_rad(10*sin(glfwGetTime())), (vec3){2.0f, -2.0f, 0.0f});
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);

        // render
        glBindVertexArray(vao);
        for (int i=0; i<10; i++)
        {
            glm_mat4_identity(model);
            glm_translate(model, cube_pos[i]);
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle)*(float)glfwGetTime(), (vec3){1.0f, 0.3f, 0.5f});
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)model);
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