#include "input.h"

static float speed = 0.5f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    printf("framebuffer_size_callback: not implemented yet!\n");
}

static double last_x=0, last_y=0;
void mouse_callback(GLFWwindow* window, double x, double y)
{
    float sensitivity = 0.1f;
    double xoff, yoff;
    xoff = x-last_x; yoff = y-last_y;
    state.camera.yaw += xoff*sensitivity;
    state.camera.pitch += yoff*sensitivity;

    // prevent impossible movements
    if (state.camera.pitch > 89.0f) state.camera.pitch = 89.0f;
    if (state.camera.pitch < -89.0f) state.camera.pitch = 89.0f; 
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    printf("scroll_callback: not implemented yet!\n");
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    // movement
    if (glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        vec3 mul; glm_vec3_scale(state.camera.z, 
                                 speed*state.delta_time, 
                                 mul);
        glm_vec3_add(state.camera.pos, mul, state.camera.pos);
    }
    if (glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        vec3 mul; glm_vec3_scale(state.camera.z, 
                                 speed*state.delta_time,
                                 mul);
        glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
    }
    if (glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(state.camera.z, state.camera.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, speed*state.delta_time, mul);
        glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
    }
    if (glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(state.camera.z, state.camera.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, speed*state.delta_time, mul);
        glm_vec3_add(state.camera.pos, mul, state.camera.pos);
    }
    if (glfwGetKey(window, 'O') == GLFW_PRESS)
        state.camera.ortho = 1;
    if (glfwGetKey(window, 'P') == GLFW_PRESS)
        state.camera.ortho = 0;
}