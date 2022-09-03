#include <stdio.h>

#include "src/global.h"

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initApplicationGlobalState( 800, 600, "testing..." );
    unsigned int sp = 0;
    sp = createShaderProgram(
        loadShader("res/glsl/vs.vs", GL_VERTEX_SHADER),
        loadShader("res/glsl/light.fs", GL_FRAGMENT_SHADER)
    );
    initCamera(false, (vec3){ 0.2f,  1.8f,  5.3f}, 
                      (vec3){ 1.0f,  0.0f,  0.0f}, 
                      (vec3){ 0.0f,  1.0f,  0.0f}, 
                      (vec3){ 0.01f, 0.2f, 1.0f},
               45.0f, 25.f, 80.f, 1.f, sp);
    

    struct model m = {0};
    m = loadModel("res/models/backpack/backpack.obj");

#include "src/render/shading.h"
    initShading(sp);
    glm_vec3_copy((vec3){-0.2f, -1.0f, -0.3f}, directional_light[0].direction);
    glm_vec3_copy((vec3){ 0.2f,  0.2f,  0.3f}, directional_light[0].ambient);
    glm_vec3_copy((vec3){ 1.0f,  1.0f,  1.0f}, directional_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.5f,  0.5f,  1.0f}, directional_light[0].specular);
    updateDirectionalLight(0);

    // tmp set material.shininess to 32
    unsigned int material_shininess_loc = 
        glGetUniformLocation(sp, "material.shininess");
    glUseProgram(sp);
    glUniform1f(material_shininess_loc, 32.f);

    return main_loop(sp, m); 
}