/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src/gl.h"
#include "src/utils.h"

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, 800, 600, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, 800, 600);

    // shaders
    // OpenGL guranties at least 16 4-component vertex attrbutes
    int nAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    printf("Max n of vertex attrib: %d\n", nAttributes); // output: 16

    unsigned int shader_prgrm, VAO, VBO, EBO;
    shader_prgrm = 
    linkShaders2program(2, 
                        FILE2shader("res/GLshaders.glsl/container.vs",
                                    GL_VERTEX_SHADER),
                        FILE2shader("res/GLshaders.glsl/container.fs",
                                    GL_FRAGMENT_SHADER)); 
    

    float vertices[] = {
        // positions        // colors         // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int texture = FILE2texture("res/textures/jade.jpg", GL_RGB,
                                        GL_TEXTURE_2D);
    unsigned int texture2 = FILE2texture("res/textures/awesomeface.png", GL_RGBA,
                                        GL_TEXTURE_2D);

    setTextureParam(4, texture, GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT,
                    GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex array attributes
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    float time, br;
    glUseProgram(shader_prgrm);
    // telling openGL which sampler belongs to which texture
    setUniform(shader_prgrm, GL_INT, "texture1", 0);
    setUniform(shader_prgrm, GL_INT, "texture2", 1);

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // process
        time = glfwGetTime();
        br = sin(time)/6 + 0.3f;
        setUniform(shader_prgrm, GL_FLOAT_VEC3, "brightness", br, br, br);

        //reder shapes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture); // first texture
        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_2D, texture2); // second texture
        
        glBindVertexArray(VAO);
        glUseProgram(shader_prgrm);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    glfwTerminate();

    return 0;
}
