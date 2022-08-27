#include "camera.h"
#include <math.h>
#include <cglm/cglm.h>

static struct camera last_cam = {0};

const float NEAR_PLANE = 0.1f, FAR_PLANE = 100.0f;

int updateCamera(struct camera *camera)
{
    int updates=0;
    if (last_cam.yaw + last_cam.pitch != 
            camera->yaw + camera->pitch ||
        last_cam.pos[0] + last_cam.pos[1] + last_cam.pos[2] != 
            camera->pos[0] + camera->pos[1] + camera->pos[2]   )
    {
        // update view matrix
        camera->z[0] = cos(camera->yaw) * cos(camera->pitch);
        camera->z[1] = sin(camera->pitch);
        camera->z[2] = sin(camera->yaw) * cos(camera->pitch);

        vec3 sum; glm_vec3_add(camera->pos, camera->z, sum);
        glm_lookat(camera->pos, sum, camera->y, camera->view); 
        updates+=1;
    }

    if (last_cam.ortho != camera->ortho ||
        last_cam.fov != camera->fov     ||
        last_cam.ratio != camera->ratio )
    {
        // update projection matrix
        if (camera->ortho)
            // TODO change to make it variable or use constants
            glm_ortho(-2.0f, 2.0f, -2.0f, 2.0f, NEAR_PLANE, FAR_PLANE, 
                camera->projection);
        else
            glm_perspective(camera->fov, camera->ratio, NEAR_PLANE, FAR_PLANE, 
                camera->projection);
        updates+=2;
    }
    
    if (updates)
        last_cam = *camera;

    return updates;
}