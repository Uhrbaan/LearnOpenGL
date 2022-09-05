#ifndef GLOBAL_H
#define GLOBAL_H

#include <cglm/cglm.h>
#include <assert.h>
#include "render/opengl.h"
#include "window/glfw.h"
#include "visual/camera.h"
#include "model/model.h"
#include "utils/utils.h"

struct game_data
{
    float delta_time;
    float speed;

    struct window window;
    struct camera camera;
    int input_mode;
};
extern struct game_data state;                                                  // dont forget to declare variable 
                                                                                // in c file and then use 'extern'
// initialises the window and camera
void initGLnGLFW(int win_w, int win_h, char *title);
void initCamera(bool ortho_projection, vec3 cam_pos, 
                vec3 cam_x, vec3 cam_y, vec3 cam_z,
                float fov, float yaw, float pitch, float roll,
                unsigned int shader_program);
// returns 1 if an error occurs
int main_loop(unsigned int shader_program, struct model model);

void drawModel(struct model m, unsigned int shader_program);

void generateVAO(struct mesh *mesh);

#endif