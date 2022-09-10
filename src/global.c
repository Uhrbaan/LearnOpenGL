#include "global.h"
#include "game/input/input.h"
#include "render/shading.h"
#include "window/window.h"

struct game_data state;

void initGlobalState(int window_w, int window_h, char *title)
{
    void *getProcAdress_fn;
    state.window = initWindow(window_w, window_h, title, &getProcAdress_fn);
    initOpenGL(0, 0, window_w, window_h, getProcAdress_fn);
    // state.camera = initCamera;
}
static float last_frame = 0;

// depth testing [file](/home/uhrbaan/Documents/code/com.learnopengl/depth-testing.md)
// stencil testing [file](/home/uhrbaan/Documents/code/com.learnopengl/stencil-testing.md)