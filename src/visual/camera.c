#include "camera.h"
#include <math.h>
#include <cglm/cglm.h>
#include <string.h>
#include "../render/opengl.h"

#define cfp (const float *)

struct camera_uniform {unsigned int model, view, projection, pos;};

static struct camera         last_cam = {0};
static struct camera_uniform camera_uniform = {0};

const float NEAR_PLANE = 0.1f, FAR_PLANE = 100.0f;

struct camera initCamera(bool ortho_projection, int window_w, int window_h,
                         vec3 cam_pos, vec3 cam_x, vec3 cam_y, vec3 cam_z,
                         float fov, float yaw, float pitch, float roll,
                         unsigned int shader_program)
{
    struct camera camera = {0};
    camera.ortho  = ortho_projection;
    camera.ratio  = (float)window_w/(float)window_h;
    memcpy   (camera.pos, cam_pos, sizeof(vec3));
    memcpy   (camera.x,   cam_x, sizeof(vec3));
    memcpy   (camera.y,   cam_y, sizeof(vec3));
    memcpy   (camera.z,   cam_z, sizeof(vec3));
    camera.fov    = fov;
    camera.yaw    = yaw; 
    camera.pitch  = pitch;
    camera.roll   = roll;

    camera_uniform.model      = glGetUniformLocation(shader_program, "model");
    camera_uniform.view       = glGetUniformLocation(shader_program, "view");
    camera_uniform.projection = glGetUniformLocation(shader_program, "projection");
    camera_uniform.pos        = glGetUniformLocation(shader_program, "camera_position");

    return camera;
}


void updateCamera(struct camera *camera, unsigned int shader_program)
{
    if (!memcmp(&last_cam, camera, sizeof(struct camera))) return;

    (*camera).z[0] = cos((*camera).yaw) * cos((*camera).pitch);
    (*camera).z[1] = sin((*camera).pitch);
    (*camera).z[2] = sin((*camera).yaw) * cos((*camera).pitch);
    // float ry = glm_rad((*camera).yaw), rp = glm_rad((*camera).pitch);
    // (*camera).z[0] = cos(ry) * cos(rp);
    // (*camera).z[1] = sin(rp);
    // (*camera).z[2] = sin(ry) * cos(rp);

    vec3 sum; 
    glm_vec3_add ((*camera).pos, (*camera).z, sum);
    glm_lookat   ((*camera).pos, sum, (*camera).y, (*camera).view);             // update view matrix

    // update projection matrix
    ((*camera).ortho == true)
        ? glm_ortho       (-2.0f, 2.0f, -2.0f, 2.0f, NEAR_PLANE, FAR_PLANE, 
                           (*camera).projection)
        : glm_perspective (glm_rad((*camera).fov), (*camera).ratio, NEAR_PLANE, FAR_PLANE, 
                           (*camera).projection);

    last_cam = *camera;
    glUseProgram(shader_program);
    glUniformMatrix4fv(camera_uniform.model,      1, 0, cfp (*camera).model);
    glUniformMatrix4fv(camera_uniform.view,       1, 0, cfp (*camera).view);
    glUniformMatrix4fv(camera_uniform.projection, 1, 0, cfp (*camera).projection);
    glUniform3fv      (camera_uniform.pos,        1,    cfp (*camera).pos);

    last_cam = *camera;
}