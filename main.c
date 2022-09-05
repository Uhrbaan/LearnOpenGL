#include <stdio.h>

#include "src/global.h"

int main(int argc, const char *argv[])
{
    printf("hello world!\n");
    initGLnGLFW( 800, 600, "testing..." );
    unsigned int sp = 0;
    sp = loadShaderProgram("res/glsl/vs.vs", "res/glsl/fs.fs");
    initShading(sp);
    initCamera(false, (vec3){ 0.2f,  1.8f,  5.3f}, 
                      (vec3){ 1.0f,  0.0f,  0.0f}, 
                      (vec3){ 0.0f,  1.0f,  0.0f}, 
                      (vec3){ 0.01f, 0.2f, 1.0f},
               45.0f, 25.f, 80.f, 1.f, sp);
    

    struct model m = {0};
    m = loadModel("res/models/backpack/backpack.obj");

#include "src/render/shading.h"
    glm_vec3_copy((vec3){-0.2f, -1.0f, -0.3f}, directional_light[0].direction); // directional light source
    glm_vec3_copy((vec3){ 0.2f,  0.2f,  0.2f}, directional_light[0].ambient);
    glm_vec3_copy((vec3){ 1.0f,  0.0f,  0.0f}, directional_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.5f,  0.5f,  1.0f}, directional_light[0].specular);
    updateDirectionalLight(0, sp);

    glm_vec3_copy((vec3){ 3.0f, 2.0f, 4.0f}, point_light[0].position);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f}, point_light[0].ambient);
    glm_vec3_copy((vec3){ 0.0f, 1.0f, 0.0f}, point_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.0f, 1.0f, 0.0f}, point_light[0].specular);
    point_light[0].constant  = 1.0f;                                            // configurations for 32 units
    point_light[0].linear    = 0.09f;
    point_light[0].quadratic = 0.032f;
    updatePointLight(0, sp);

    glm_vec3_copy((vec3){ -.2f, 3.5f, -.5f}, spot_light[0].position);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, -1.f}, spot_light[0].direction);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f}, spot_light[0].ambient);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 1.0f}, spot_light[0].diffuse);
    glm_vec3_copy((vec3){ 0.0f, 0.0f, 1.0f}, spot_light[0].specular);
    spot_light[0].constant  = 1.0f;
    spot_light[0].linear    = 0.045f;
    spot_light[0].quadratic = 0.0075f;
    spot_light[0].cutoff    = cos(glm_rad(12.5f));
    spot_light[0].outer_cutoff = cos(glm_rad(17.5f));
    updatespotLight(0, sp);

    // tmp set material.shininess to 32
    unsigned int material_shininess_loc = 
        glGetUniformLocation(sp, "material.shininess");
    glUseProgram(sp);
    glUniform1f(material_shininess_loc, 32.f);

    main_loop(sp, m); 

    return 0; 
}