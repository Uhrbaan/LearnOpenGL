#include <string.h>
#include "camera.h"

static float last_angle=0, last_fov=0, last_pos_sum=0;
static bool changed=false;

camera initCamera(vec3 pos, vec3 front, vec3 up, int w, int h, float speed)
{
    struct camera camera = {0};
    memcpy(camera.pos, pos, sizeof(vec3));
    memcpy(camera.z, front, sizeof(vec3));
    memcpy(camera.y, up, sizeof(vec3));

    camera.ortho = 0;
    camera.w = w;
    camera.h = h;
    camera.speed = speed;
    camera.fov = 45.0f;
    return camera;
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
        last_pos_sum = cam.pos[0]+cam.pos[1]+cam.pos[2];
        changes++;
    }
    if (!cam.ortho)
    {
        if (last_fov != cam.fov || changed == true)
        {
            glm_perspective(glm_rad(cam.fov), (float)cam.w/(float)cam.h, 
                            0.1f, 100.0f, cam.projection.m);
            updateUniformMatrix(cam.projection, 0);
            last_fov = cam.fov;
            changes++;
            changed=false;
        }
    }
    else 
    {
        if (!changed)
        {
            glm_ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f, cam.projection.m);
            updateUniformMatrix(cam.projection, 0);
            changes++;
            changed = true;
        }
    }

    if (changes)
    {
        vec3 sum; glm_vec3_add(cam.pos, cam.z, sum);
        glm_lookat(cam.pos, sum, cam.y, cam.view.m);
        updateUniformMatrix(cam.view, 0);
    }
}