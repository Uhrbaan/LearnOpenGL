#ifndef camera_h
#define camera_h

#include <cglm/cglm.h>

struct camera {
    vec3 pos;
    vec3 front;
    vec3 target;
    vec3 direction;
    vec3 right;
    vec3 up;
    float speed;
};

extern struct camera cam;

extern float delta_time; // helps to balance out slow & fast frames
extern float last_frame;

struct camera initCamera();
void moveCamera(vec3 pos, vec3 front, vec3 up, mat4 d);

#endif