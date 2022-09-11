#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include <cglm/cglm.h>
struct camera
{
    bool ortho;
    float ratio;
    vec3 pos, x, y, z;
    float fov, yaw, pitch, roll;
    mat4 model, view, projection;
};

struct camera initCamera(bool ortho_projection, int window_w, int window_h,
                         vec3 cam_pos, vec3 cam_x, vec3 cam_y, vec3 cam_z,
                         float fov, float yaw, float pitch, float roll);
void updateCamera(struct camera *camera, unsigned int shader_program);

#endif