/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src/gl.h"
#include "src/utils.h"
#include "src/stb_image.h"

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

    unsigned int shader_prgrm, VAO, VBO;
    shader_prgrm = 
    linkShaders2program(2, 
                        FILE2shader("res/GLshaders.glsl/vert_triangle.glsl",
                                    GL_VERTEX_SHADER),
                        FILE2shader("res/GLshaders.glsl/frag_triangle.glsl",
                                    GL_FRAGMENT_SHADER)); 
    

    float vertices[] = {
        // vertices
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    float texture_coo[] = { // x&y entre 0..1, 0|0 en bas à gauche
        0.0f, 0.0f, // en bas gauche
        1.0f, 0.0f, // en bas droite
        0.5f, 1.0f  // centre haut
    };

    unsigned int texture = FILE2texture("res/textures/container.jpg", 
                                        GL_TEXTURE_2D);

    /* modifie l'apparence de la texture si les coordonnées ne map pas l'objet
     * options : GL_REPEAT (défaut), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, 
     *           GL_CLAMP_TO_BORDER ||  if clamp to edge is chosen:
     *  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
     *  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    /* texture filtering: ce que doit faire opengl quand les texels n'alignent
     * pas les pixels : GL_NEAREST, GL_LINEAR 
     * can be set on magnifying (GL_TEXTURE_MAG_FILTER) or minifying
     * (GL_TEXTURE_MIN_FILTER) operations */
    /* to prevent memory wastage, when small or far objects are rendered, smaler
     * textures are used (mipmaps). can be generated through glGenerateMipmaps
     * p. 58 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    

    // generate objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex array attributes
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //reder shapes
        glBindVertexArray(VAO);
        glUseProgram(shader_prgrm);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 
        glfwSwapBuffers(window);
        glfwPollEvents();
        msleep(16);
    }

    glfwTerminate();

    return 0;
}
