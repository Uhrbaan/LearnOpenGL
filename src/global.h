#ifndef GLOBAL_H
#define GLOBAL_H

#include <cglm/cglm.h>
#include <assert.h>
#include "render/opengl.h"
#include "window/window.h"
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
void initGlobalState(int win_w, int win_h, char *title);
int main_loop(unsigned int shader_program, struct model model);

void drawModel(struct model m, unsigned int shader_program);

#endif