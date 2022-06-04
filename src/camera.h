#ifndef camera_h
#define camera_h

#include <cglm/cglm.h>
#include "gl.h" 

typedef struct camera {
    int ortho, w, h;
    vec3 pos, x, y, z;
    float speed, fov, yaw, pitch, roll;
    mat4wloc view, projection;
} camera;

extern camera cam;

extern float delta_time; // helps to balance out slow & fast frames
extern float last_frame;

camera initCamera(vec3 pos, vec3 target, vec3 up, int w, int h, float speed);
void updateCamera();
void angleVec3(vec3 direction, float yaw, float pitch);

#endif