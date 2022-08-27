#ifndef GLOBAL_H
#define GLOBAL_H

#include <cglm/cglm.h>
#include <assert.h>
#include "window/glfw.h"
#include "visual/camera.h"
#include "model/mesh.h"

/**
 * a struct that keeps track of all variables necesseary to run the game.
 * is stored in a global value shared through all the files
 * make it so that the types dont require too many dependecies
 */
struct game_state
{
    float delta_time;
    float speed;

    struct window window;
    struct camera camera;
};
struct game_state state;
// global variable
#include "input.h"

void initApplicationGlobalState(int win_w, int win_h, char *title,
                                bool ortho_projection, vec3 cam_pos, 
                                vec3 cam_x, vec3 cam_y, vec3 cam_z,
                                float fov, float yaw, float pitch, float roll)
{
    memset(&state, 0, sizeof(struct game_state));
    // window
    int result=0;
    state.window.w = win_w;
    state.window.h = win_h;
    strncpy(state.window.title, title, 199);
    result=initGLFW(&state.window.glfw_window, win_w, win_h, title, NULL, NULL,
                    mouse_callback, scroll_callback, framebuffer_size_callback);
    initGlad(0, 0, win_w, win_h, glfwGetProcAddress);
    assert(!result); // TODO a better error handleing

    // camera
    state.camera.ortho = ortho_projection;
    state.camera.ratio = (float)win_w/(float)win_h;
    memcpy(state.camera.pos, cam_pos, sizeof(vec3));
    memcpy(state.camera.x, cam_x, sizeof(vec3));
    memcpy(state.camera.y, cam_y, sizeof(vec3));
    memcpy(state.camera.z, cam_z, sizeof(vec3));
    state.camera.fov = fov;
    state.camera.yaw = yaw; 
    state.camera.pitch = pitch;
    state.camera.roll = roll; // will probably not be used
    updateCamera(&state.camera);
    glm_mat4_identity(state.camera.model);
    setGlTextureId_fn(loadTexture);
}

void generateMeshVAO(struct mesh *m)
{
    m->vao = genVao(&m->vbo, &m->ebo,
                    &m->vertices, sizeof(struct vertex)*m->n_vert,
                    &m->indices, sizeof(unsigned int)*m->n_indi, 
                    sizeof(struct vertex));
}

#endif