#include <stdio.h>

#include "src/global.h"

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initApplicationGlobalState( 800, 600, "testing..." );
    unsigned int sp = 0;
    sp = createShaderProgram(
        loadShader("res/glsl/vs.vs", GL_VERTEX_SHADER),
        loadShader("res/glsl/fs.fs", GL_FRAGMENT_SHADER)
    );
    initCamera(false, (vec3){0.0f, 0.0f, 0.0f}, 
                      (vec3){1.0f, 0.0f, 0.0f}, 
                      (vec3){0.0f, 1.0f, 0.0f}, 
                      (vec3){0.0f, 0.0f,-1.0f},
               45.0f, 0.0f, 0.0f, 0.0f, sp);
    

    struct model m = {0};
    m = loadModel("res/models/backpack/backpack.obj");

    state.model = m;
    state.shader_program = sp;

    return main_loop(sp); 
}