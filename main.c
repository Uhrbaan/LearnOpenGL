/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "src/gl.h"
#include "src/utils.h"

int main(int argc, char const *argv[])
{
    GLFWwindow *window=NULL;
    if (initGLFW(&window, 800, 600, "Learn Opengl", NULL, NULL))
        return 1;
    
    initGLAD(0, 0, 800, 600);

// <shaders>
    unsigned int vert_shader = FILE2shader("res/GLshaders.glsl/vert_triangle.glsl",
                                           GL_VERTEX_SHADER);
    unsigned int frag_shader = FILE2shader("res/GLshaders.glsl/frag_triangle.glsl",
                                           GL_FRAGMENT_SHADER);

    // shader program -> a linked version of multiple shaders
    unsigned int shader_program = linkShaders2program(2, vert_shader, 
                                                      frag_shader);
    if (!shader_program)
        return 1;
// </shaders>
// <triangle>
    float vertices[] = {   
        /* points nécessaires pour dessiner le triangle
         * NOTE: (0, 0) est au centre de l'écran, l'axe y est positif vers le
         * haut */
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.0f,  0.5f, 0.0f
    };

    // generating objects
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    /* we need to tell OpenGL how to interpret the vertex data
     * Arguments:
     *  1. sets the loacation of the vertex attribute to 0 
     *     └> layout (location=0)
     *     va donc modifier les vertex à la position 0
     *  2. spécifie la taille du vertex. comme c'est un vec3 la taille est de 3
     *  3. le type de data passé dans le vecteur (un vec* est composé de float)
     *  4. if we want to normalize the data
     *     └> modifierait les uint à 0, sint à -1, float à 1
     *  5. appelé 'stride', c'est le nombre de byte qui sépare chaque vertex
     *  6. offest where the data begins, needs a (void*) cast */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
// </triangle>


    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set entire window color
        glClear(GL_COLOR_BUFFER_BIT);
        // possible bit buffers to set:
        //  GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); /* glDrawArrays(primitive_shape, 
                                           * start_index, n_vertices to draw) */
        // glBindVertexArray(0); // pas nécessaire de l'appeler à chaque fois
        // swapping buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    glfwTerminate();

    return 0;
}
