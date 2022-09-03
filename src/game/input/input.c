#include "../../global.h"
#include "input.h"


struct cursor{
    float lastx, lasty;
    float sensitivity;
};
struct cursor cursor = {.0f, .0f, .005f};
void mouse_callback(GLFWwindow* window, double x, double y)
{
    float offx = (x-cursor.lastx)*cursor.sensitivity;
    float offy = (y-cursor.lasty)*cursor.sensitivity;
    cursor.lastx = x;
    cursor.lasty = y;

    state.camera.yaw += offx;
    state.camera.pitch -= offy;

    // to prevent the lookat function to flip the camera, we need to prevent the 
    // user to have an angle greater than 89°
    if(state.camera.pitch > 89.0f)
        state.camera.pitch = 89.0f;
    else if (state.camera.pitch < -89.0f)
        state.camera.pitch = -89.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    input_context_free:
    switch (key)                                                                // key combinations that are valid 
    {                                                                           // despite the input_context
        case 'Q':
            if (action == GLFW_PRESS && mods == GLFW_MOD_CONTROL)
                glfwSetWindowShouldClose(window, 1);
            break;
        case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, 1);
            break;
        default:
            goto input_context_specific;
    }

    input_context_specific:
    switch (state.input_mode)
    {
        case general:
            switch (key)
            {
                case GLFW_KEY_ESCAPE:
                    printf("show pause screen...\n");
                    break;
                
                default:
                    break;
            }
            break;
        
        default:
            printf("this input context is not used\n");
            break;
        }
}

float speed = 4.0f;
void movement(GLFWwindow *window)
{
    // forward
    if (glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        vec3 mul;
        glm_vec3_scale(state.camera.z, speed * state.delta_time, mul);
        glm_vec3_add(state.camera.pos, mul, state.camera.pos);
    }
    // backward
    if (glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        vec3 mul;
        glm_vec3_scale(state.camera.z, speed * state.delta_time, mul);
        glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
    }
    // left
    if (glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        vec3 mul;
        glm_cross(state.camera.z, state.camera.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, speed * state.delta_time, mul);
        glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
    }
    // right
    if (glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        vec3 mul;
        glm_cross(state.camera.z, state.camera.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, speed * state.delta_time, mul);
        glm_vec3_add(state.camera.pos, mul, state.camera.pos);
    }
    // up
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        state.camera.pos[1] += speed * state.delta_time;
    // down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        state.camera.pos[1] -= speed * state.delta_time;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        state.camera.fov -= (float)yoffset*.5f;
        if (state.camera.fov < 1.0f)
            state.camera.fov = 1.0f;
        else if (state.camera.fov > 80.0f)
            state.camera.fov = 80.0f;
    }
    else
    {
        speed += yoffset*.2;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    state.window.w = width;
    state.window.h = height;
    glViewport(0, 0, width, height);                                            // resets the size of the rendering window
    state.camera.ratio = (float)width/(float)height;
} 