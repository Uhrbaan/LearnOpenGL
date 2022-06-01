#include <string.h>
#include "camera.h"

struct camera initCamera()
{
    // struct camera cam = {0};
    // memcpy(cam.pos, pos, sizeof(pos));
    // memcpy(cam.target, target, sizeof(target));
    // glm_vec3_sub(cam.pos, cam.target, cam.direction);
    // glm_normalize(cam.direction);
    // glm_cross(up, cam.direction, cam.right); // vektor produkt up ⊥ x
    // glm_normalize(cam.right);
    // glm_cross(cam.direction, cam.right, cam.up);
    return (struct camera){0};
}

void moveCamera(vec3 pos, vec3 front, vec3 up, mat4 d)
{
    vec3 sum; glm_vec3_add(pos, front, sum);
    glm_lookat(pos, sum, up, d);
}

void angleVec3(vec3 direction, float yaw, float pitch)
{
    direction[0] = cos(yaw) * cos(pitch);
    direction[1] = sin(pitch);
    direction[2] = sin(yaw) * cos(pitch);
}