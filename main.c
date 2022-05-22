/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
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
    if (initGLFW(&window, 800, 800, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, 800, 800);

    // <shaders>
    unsigned int shader_prgrm, VAO, VBO, EBO;
    shader_prgrm = glCreateProgram();
    if (linkShaders(shader_prgrm, 2, 
                    FILE2shader("res/GLshaders.glsl/shader.vs",
                                GL_VERTEX_SHADER), 
                    FILE2shader("res/GLshaders.glsl/shader.fs", 
                                GL_FRAGMENT_SHADER)))
        return 1;

    float vertices[] = {
        // vertices         // colors         // texture coo
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top left
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top right
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // botom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f    // botom left
    };

    unsigned int indices[] = {
        1, 2, 4,
        2, 3, 4
    };

    unsigned int texture = FILE2texture("res/textures/safe_landing.jpg", 
                                         GL_RGB, GL_TEXTURE_2D, false);
    glBindTexture(GL_TEXTURE_2D, texture);
    setTextureParam(4, texture, GL_TEXTURE_2D, 
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // genrating buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // binding vertex array -> keeps track of vertices

    // vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    // element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    // vertex array attributes
    // position attribute   -> repeats every 8 floats 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute      -> repeats every 8 floats and starts wit 3 offset
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinates  -> repeats every 8 floats and starts at the 6th
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), 
                          (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // setup to modify render over time
    float time_value;
    unsigned int transform_uniform;
    mat4 m = GLM_MAT4_IDENTITY_INIT;
    
    glUseProgram(shader_prgrm);
    transform_uniform = glGetUniformLocation(shader_prgrm, "transform");

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // process
        time_value = glfwGetTime();
        glm_mat4_identity(m);
        glm_translate(m, (vec3){sin(time_value)/2, cos(time_value)/2, 0.0f});
        glm_rotate(m, time_value, (vec3){0.0f, 0.0f, 1.0f});

        //reder shapes
        glUseProgram(shader_prgrm);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        glUniformMatrix4fv(transform_uniform, 1, GL_FALSE, (const float*)m);
        
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