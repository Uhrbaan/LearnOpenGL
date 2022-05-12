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

// <rectangle>
    float vertices[] = {   
        /* points nécessaires pour dessiner 2 triangles, les overlaps sont omis
         * NOTE: (0, 0) est au centre de l'écran, l'axe y est positif vers le
         * haut */
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        /* indique les vertices que chaque triangle doit utiliser */
        0, 1, 3,            // premier triangle
        1, 2, 3             // deuxième triangle
    };

    // generating objects
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // binding vertex array object to keep track of all 
                            // the context changes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);             // Binding vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), // allocating buffer
                 vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); /* element array buffer, to
                                                 * minimize space usage by not
                                                 * repeating same vertices and
                                                 * replacing them w/ indecies */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 
                 indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,     // vertex attribute 
                          3*sizeof(float), (void*)0);   // configuration
    glEnableVertexAttribArray(0);


// </rectangle>

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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES,    // shape 
                       6,               // number of indices
                       GL_UNSIGNED_INT, // type of indices
                       0);              // offset
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    glfwTerminate();

    return 0;
}
