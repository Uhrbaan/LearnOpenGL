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
    initCamera(false, (vec3){0.0f, 0.0f, 0.0f}, 
                      (vec3){1.0f, 0.0f, 0.0f}, 
                      (vec3){0.0f, 1.0f, 0.0f}, 
                      (vec3){0.0f, 0.0f,-1.0f},
               45.0f, 0.0f, 0.0f, 0.0f, sp);
    

    struct model m = {0};
    m = loadModel("res/models/backpack/backpack.obj");

#include "src/render/shading.h"
    initShading(sp);
    glm_vec3_copy((vec3){-0.2f, -1.0f, -0.3f}, directional_light[0].direction);
    glm_vec3_copy((vec3){ 1.0f,  1.0f,  1.0f}, directional_light[0].direction);
    glm_vec3_copy((vec3){ 1.0f,  1.0f,  1.0f}, directional_light[0].direction);
    glm_vec3_copy((vec3){ 1.0f,  1.0f,  1.0f}, directional_light[0].direction);
    updateDirectionalLight(0);
    return main_loop(sp, m); 
}