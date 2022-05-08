/* Apprendre le OpenGL (v. )
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

    // hello triangle
    float vertecies[] = {   
        /* points nécessaires pour dessiner le triangle
         * NOTE: (0, 0) est au centre de l'écran, l'axe y est positif vers le
         * haut */
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // vertex buffer object
    unsigned int VBO;// vertex buffer object id, used to store vertecies
    glGenBuffers(1, &VBO);
    /* binding the buffer to an ARRAY_BUFFER target, any other configurations on
     * the ARRAY_BUFFER target made on currently bound buffer (here: VBO)*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /* put the vertex data in the target
     * last argument specifies how graphics card handels data, 3 forms:
     *  - GL_STREAM_DRAW: the data is set only once and used by the GPU at most
     *    a few times. 
     *  - GL_STATIC_DRAW: the data is set only once and used many times.
     *  - GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

    // shaders
    unsigned int vert_shader = FILE2shader("res/GLshaders.glsl/vert_triangle",
                                           GL_VERTEX_SHADER);
    unsigned int frag_shader = FILE2shader("res/GLshaders.glsl/frag_triangle",
                                           GL_FRAGMENT_SHADER);

    // shader program -> a linked version of multiple shaders
    unsigned int shader_program = linkShaders2program(2, vert_shader, 
                                                      frag_shader);
    if (!shader_program)
        return 1;

    // we need to tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    /* Arguments
     *  1. sets the loacation of the vertex attribute to 0 
     *     └> layout (loacation=0)
     *     va donc modifier les vertex à la position 0
     *  2. spécifie la taille du vertex. comme c'est un vec3 la taille est de 3
     *  3. le type de data passé dans le vecteur (un vec* est composé de float)
     *  4. if we want to normalize the data
     *     └> modifierait les uint à 0, sint à -1, float à 1
     *  5. appelé 'stride', c'est le nombre de byte qui sépare chaque vertex
     *  6. offest where the data begins, needs a (void*) cast */
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set entire window color
        glClear(GL_COLOR_BUFFER_BIT);
        // possible bit buffers to set:
        //  GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT.

        // swapping buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
