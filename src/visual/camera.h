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
    mat4 view, projection, model;
    struct {
        unsigned int view, projection, model;
    } uni_loc;
};
int updateCamera(struct camera *camera);

#endif
