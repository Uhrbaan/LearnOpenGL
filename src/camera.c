#include <string.h>
#include "camera.h"

static float last_angle=0, last_fov=0, last_pos_sum=0;

struct camera initCamera(vec3 pos, vec3 target, vec3 up, int w, int h, 
                         float speed)
{
    // struct camera cam = {0};
    // memcpy(cam.pos, pos, sizeof(pos));
    // memcpy(cam.target, target, sizeof(target));
    // glm_vec3_sub(cam.pos, cam.target, cam.direction);
    // glm_normalize(cam.direction);
    // glm_cross(up, cam.direction, cam.right); // vektor produkt up ⊥ x
    // glm_normalize(cam.right);
    // glm_cross(cam.direction, cam.right, cam.up);

    memset(&cam, 0, sizeof(cam));

    memcpy(cam.pos, pos, sizeof(vec3));
    glm_vec3_sub(cam.pos, target, cam.z);
    glm_normalize(cam.z);
    memcpy(cam.y, up, sizeof(vec3));
    glm_cross(up, cam.z, cam.x);
    glm_normalize(cam.x);
    glm_cross(cam.z, cam.x, cam.y);

    cam.ortho = 0;
    cam.w = w;
    cam.h = h;
    cam.speed = speed;
    cam.fov = 45.0f;

    memset(&cam, 0, sizeof(struct camera));
    return (struct camera){0};
}

void updateCamera()
{
    int changes=0;
    if (last_angle != cam.yaw+cam.pitch)
    {
        float ryaw = glm_rad(cam.yaw), rpitch = glm_rad(cam.pitch);
        cam.z[0] = cos(ryaw) * cos(rpitch);
        cam.z[1] = sin(rpitch);
        cam.z[2] = sin(ryaw) * cos(rpitch);
        last_angle = cam.yaw+cam.pitch;
        changes++;
    }

    if (last_pos_sum != cam.pos[0]+cam.pos[1]+cam.pos[2])
    {
        vec3 sum; glm_vec3_add(cam.pos, cam.z, sum);
        glm_lookat(cam.pos, sum, cam.y, cam.view.m);
        last_pos_sum = cam.pos[0]+cam.pos[1]+cam.pos[2];
        changes++;
    }
    if (!cam.ortho)
    {
        if (last_fov != cam.fov)
        {
            glm_perspective(glm_rad(cam.fov), (float)cam.w/(float)cam.h, 
                            0.1f, 100.0f, cam.projection.m);
            updateUniformMatrix(cam.projection, 0);
            last_fov = cam.fov;
            changes++;
        }
    }
    else 
    {
        glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f, cam.projection.m);
        updateUniformMatrix(cam.projection, 0);
        changes++;
    }

    if (changes)
        updateUniformMatrix(cam.view, 0);
}