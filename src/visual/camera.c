#include "camera.h"
#include <math.h>
#include <cglm/cglm.h>
#include <string.h>
#include "../render/opengl.h"

#define cfp (const float *)

struct camera_uniform {unsigned int model, view, projection, pos;};

static struct camera last_cam = {0};
#define MAX_SHADER_PROGRAMS 100
static struct camera_uniform camera_uniform = {0};

const float NEAR_PLANE = 0.1f, FAR_PLANE = 100.0f;

struct camera initCamera(bool ortho_projection, int window_w, int window_h,
                         vec3 cam_pos, vec3 cam_x, vec3 cam_y, vec3 cam_z,
                         float fov, float yaw, float pitch, float roll, 
                         unsigned int sp)
{
    struct camera camera = {0};
    camera.ortho  = ortho_projection;
    camera.ratio  = (float)window_w/(float)window_h;
    glm_vec3_copy   (cam_pos, camera.pos);
    glm_vec3_copy   (cam_x,   camera.x);
    glm_vec3_copy   (cam_y,   camera.y);
    glm_vec3_copy   (cam_z,   camera.z);
    camera.fov    = fov;
    camera.yaw    = yaw; 
    camera.pitch  = pitch;
    camera.roll   = roll;

    glm_mat4_identity(camera.model);
    glm_mat4_identity(camera.view);
    glm_mat4_identity(camera.projection);

    camera_uniform.model      = glGetUniformLocation(sp, "model");
    camera_uniform.view       = glGetUniformLocation(sp, "view");
    camera_uniform.projection = glGetUniformLocation(sp, "projection");
    camera_uniform.pos        = glGetUniformLocation(sp, "camera_position");

    return camera;
}

void updateCamera(struct camera *camera, unsigned int shader_program)
{
    if (!memcmp(&last_cam, camera, sizeof(struct camera))) return;

    (*camera).z[0] = cos((*camera).yaw) * cos((*camera).pitch);
    (*camera).z[1] = sin((*camera).pitch);
    (*camera).z[2] = sin((*camera).yaw) * cos((*camera).pitch);

    vec3 sum;
    glm_vec3_add ((*camera).pos, (*camera).z, sum);
    glm_lookat   ((*camera).pos, sum, (*camera).y, (*camera).view);             // update view matrix

    // update projection matrix
    ((*camera).ortho == true)
        ? glm_ortho       (-2.0f, 2.0f, -2.0f, 2.0f, NEAR_PLANE, FAR_PLANE, 
                           (*camera).projection)
        : glm_perspective ((*camera).fov, (*camera).ratio, NEAR_PLANE, FAR_PLANE, 
                           (*camera).projection);

    last_cam = *camera;
    glUseProgram(shader_program);
    glUniformMatrix4fv(camera_uniform.model,      1, 0, cfp (*camera).model);
    glUniformMatrix4fv(camera_uniform.view,       1, 0, cfp (*camera).view);
    glUniformMatrix4fv(camera_uniform.projection, 1, 0, cfp (*camera).projection);
    glUniform3fv      (camera_uniform.pos,        1,    cfp (*camera).pos);

    last_cam = *camera;
}
// FIXME corrupted model showing -> indices ? vao ?

/** NOTE
 * if you see shader programs multiplied or divided by 3, it is because shader
 * programs (the way they are loaded by this implementation) increment by 3
 * (2 for the compiled shader files + 1 for the program). this might change. 
 * 
 */