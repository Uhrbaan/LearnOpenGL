/* Apprendre le OpenGL (v. 4.6 -> core 3.3)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cglm/cglm.h>

#include "src/gl.h"
#include "src/utils/utils.h"
#include "src/utils/matrix.h"

int main(int argc, char const *argv[])
{
//     GLFWwindow *window=NULL;
//     if (initGLFW(&window, 800, 600, "Learn Opengl", NULL, NULL))
//         return 1;
    
//     initGLAD(0, 0, 800, 600);

//     // shaders
//     // OpenGL guranties at least 16 4-component vertex attrbutes
//     int nAttributes;
//     glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
//     printf("Max n of vertex attrib: %d\n", nAttributes); // output: 16

//     unsigned int shader_prgrm, VAO, VBO;
//     shader_prgrm = 
//     linkShaders2program(2, 
//                         FILE2shader("res/GLshaders.glsl/vert_triangle.glsl",
//                                     GL_VERTEX_SHADER),
//                         FILE2shader("res/GLshaders.glsl/frag_triangle.glsl",
//                                     GL_FRAGMENT_SHADER)); 
    

//     float vertices[] = { // with color data
//         // vertices         // colors
//          0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//          0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
//     };

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     // vertex array attributes
//     // position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
    
//     // color attribute
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
//                           (void*)(3*sizeof(float)));
//     glEnableVertexAttribArray(1);
// /*  glVertexAttribPointer(attribute position, number of elements, element type, 
//                           normalize, stride (bytes till nex first element),
//                           (void*)(offset)) */

//     // setup to change brightness through time
//     float time_value, brightness;
//     // int vertex_color_location = glGetUniformLocation(shader_prgrm, "add_color");

    vec4 v = {1.0f, 0.0f, 0.0f, 1.0f};
    mat4 m = mat4_identity_matrix;
    // modifying the matrix m to do following transformations
    glm_rotate(m, glm_rad(90.0f), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(m, (vec3){2.0f, 2.0f, 2.0f});
    glm_translate(m, (vec3){1.0f, 1.0f, 1.0f});

    glm_mat4_mulv(m, v, v);
    printf("vec: (%f|%f|%f)\n", v[0], v[1], v[2]);
    


    // while(!glfwWindowShouldClose(window))
    // {
    //     // input
    //     processInput(window);

    //     // rendering
    //     // clear screen
    //     glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     // process
    //     time_value = glfwGetTime();
    //     brightness = (sin(time_value) / 2.0f);


    //     //reder shapes
    //     glBindVertexArray(VAO);
    //     glUseProgram(shader_prgrm);
    //     // glUniform4f(vertex_color_location, brightness, brightness, brightness,
    //     //             brightness);
    //     setUniform(shader_prgrm, GL_FLOAT_VEC4, "add_color", brightness, 
    //                brightness, brightness, brightness);
    //     glDrawArrays(GL_TRIANGLES, 0, 3);

    //     // 
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    //     msleep(16);
    // }

    // glfwTerminate();

    return 0;
}
