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

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, 800, 600, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, 800, 600);

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
     * -> V_clip = M_projection · M_view · M_model ·V_local
     *  ! matrix multiplication reads from right to left
     */
    
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    // glm_rotate(model, glm_rad(-55.0f),(vec3){1.0f, 0.0f, 0.0f});
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    // translating scene forward (-z) to appear going further
    // glm_translate(view, (vec3){0.0f, 0.0f, -0.3f});
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // glm_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f, projection);

    unsigned int model_loc, view_loc, projection_loc;
    glUseProgram(shader_program);
    model_loc = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)model);
    view_loc = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);
    projection_loc = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float*)projection);

    float vertices[] = {
        // positions        // colors         // texture coords
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int vao, vbo, ebo;
    size_t stride, off_pos, off_col, off_tex;
    stride  = 8*sizeof(float);
    off_pos = 0;
    off_col = 3*sizeof(float); 
    off_tex = 6*sizeof(float); 
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo); glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_VERTEX_ARRAY, vbo);
    glBufferData(GL_VERTEX_ARRAY, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)off_pos);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)off_col);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)off_tex);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    unsigned int texture = FILE2texture("res/textures/jade.jpg",
                                        GL_RGB, GL_TEXTURE_2D);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // process
        glUseProgram(shader_program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }
 
    glfwTerminate();

    return 0;
}

/* fn help
 * glVertexAttribPointer(attribute position, 
 *                       number of elements, 
 *                       element type, 
 *                       normalize, 
 *                       stride,
 *                       offset) */