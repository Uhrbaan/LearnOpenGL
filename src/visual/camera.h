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

/**
 * @brief initializes a camera object.
 * 
 * @param ortho_projection 1 if orthographic projection
 * @param window_w window width
 * @param window_h window height
 * @param cam_pos camera position
 * @param cam_x camera x-axes
 * @param cam_y camera y-axes
 * @param cam_z camera z-axes
 * @param fov fiel of view (in Rad)
 * @param yaw yaw in rad
 * @param pitch pitch in rad
 * @param roll roll in rad
 * @param shader_program shader progam to init the camera uniform
 * @return struct camera 
 */
struct camera initCamera(bool ortho_projection, int window_w, int window_h,
                         vec3 cam_pos, vec3 cam_x, vec3 cam_y, vec3 cam_z,
                         float fov, float yaw, float pitch, float roll,
                         unsigned int shader_program);
/**
 * @brief initializes only a camera uniform liked to specified shader program,
 *        can be useful if not an entire camera objects has to be created.
 * 
 * @param shader_program shader program which the uniform is assigned to
 */
void initOnlyCameraUniform(unsigned int shader_program);

/**
 * @brief updates the camera uniform to that shader. Camera objects and shader
 *        programs can be mismatched (you can have a single camera object for 
 *        multiple shaders)
 * 
 * @param camera camera object to update (view, & z), shader program camera 
 *               will also be updated (can be mismatched)
 * @param shader_program shader program
 */
void updateCamera(struct camera *camera, unsigned int shader_program);

#endif