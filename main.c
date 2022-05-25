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
    unsigned int shader_program;
    shader_program = glCreateProgram();
    if (linkShaders(shader_program, 2, 
                    FILE2shader("res/GLshaders.glsl/shader.vs",
                                GL_VERTEX_SHADER), 
                    FILE2shader("res/GLshaders.glsl/shader.fs", 
                                GL_FRAGMENT_SHADER)))
        return 1;

    // glEnable(GL_TEXTURE_2D); // troubleshooting steps https://stackoverflow.com/questions/740151/what-are-the-usual-troubleshooting-steps-for-opengl-textures-not-showing
    unsigned int 
    texture1 = FILE2texture("/usr/share/wallpapers/SafeLanding/contents/images/5120x2880.jpg", 
                            GL_RGB, GL_TEXTURE_2D),
    texture2 = FILE2texture("/usr/share/wallpapers/Altai/contents/images/5120x2880.png",
                            GL_RGB, GL_TEXTURE_2D);

    float vertices[] = 
        {// positions         // colors           // texture coords
         0.7f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
         0.7f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.7f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.7f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left
        };
    unsigned int indices[] = 
        {
            0, 1, 3,
            1, 2, 3 
        };

    unsigned int VAO1, VBO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    unsigned int VAO2, VBO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    // setup to modify render over time
    float time_value;
    unsigned int transform_uniform;
    mat4 m = GLM_MAT4_IDENTITY_INIT;
    
    glUseProgram(shader_program);
    transform_uniform = glGetUniformLocation(shader_program, "transform");

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
        time_value = glfwGetTime();

        // modifications & 
        glm_mat4_identity(m);
        glm_translate(m, (vec3){sin(time_value)/2, cos(time_value)/2, 0.0f});
        glm_rotate(m, time_value, (vec3){0.0f, 0.0f, 1.0f});
        glUseProgram(shader_program);
        glUniformMatrix4fv(transform_uniform, 1, GL_FALSE, (const float*)m);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm_mat4_identity(m);
        glm_translate(m, (vec3){cos(time_value)/3, sin(time_value)/3, 0.0f});
        glm_scale(m, (vec3){0.5f, 0.5f, 1.0f});
        glm_rotate(m, -time_value, (vec3){0.0f, 0.0f, 1.0f});
        glUseProgram(shader_program);
        glUniformMatrix4fv(transform_uniform, 1, GL_FALSE, (const float*)m);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO2);
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