#include "glfw.h"
#include "../utils/utils.h"
#include <cglm/cglm.h>

void glfwErrorCallback(int error_code, const char* description)
{
    fprintf(stderr, 
            TERM_COL_ERROR("error: ")TERM_COL_INFO("%s")" error code: %d",
            description, error_code);
}

void * initGLFW(GLFWwindow **window, int width, int height, 
                const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    if (glfwInit() == GLFW_TRUE)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        *window = glfwCreateWindow(width, height, title, monitor, share);
        if (*window == NULL)
        {
            fprintf(stderr, TERM_COL_ERROR("error:") 
                    "failded to create glfw window");
            glfwTerminate();
            return  NULL;
        }

        glfwMakeContextCurrent(*window);                                        // callbacks for window
        glfwSetErrorCallback(glfwErrorCallback);  
#include "../game/input/input.h"
        glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
        glfwSetCursorPosCallback(*window, mouse_callback);
        glfwSetScrollCallback(*window, scroll_callback);
        glfwSetKeyCallback(*window, key_callback);
        
        return glfwGetProcAddress;
    }
    return NULL;
}

// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, 1);

//     // movement
//     if (glfwGetKey(window, 'W') == GLFW_PRESS)
//     {
//         vec3 mul; glm_vec3_scale(state.camera.z, 
//                                  state.camera.speed*state.window.delta_time, 
//                                  mul);
//         glm_vec3_add(state.camera.pos, mul, state.camera.pos);
//     }
//     if (glfwGetKey(window, 'S') == GLFW_PRESS)
//     {
//         vec3 mul; glm_vec3_scale(state.camera.z, 
//                                  state.camera.speed*state.window.delta_time,
//                                  mul);
//         glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
//     }
//     if (glfwGetKey(window, 'A') == GLFW_PRESS)
//     {
//         vec3 mul; 
//         glm_cross(state.camera.z, state.camera.y, mul);
//         glm_normalize(mul);
//         glm_vec3_scale(mul, state.camera.speed*state.window.delta_time, mul);
//         glm_vec3_sub(state.camera.pos, mul, state.camera.pos);
//     }
//     if (glfwGetKey(window, 'D') == GLFW_PRESS)
//     {
//         vec3 mul; 
//         glm_cross(state.camera.z, state.camera.y, mul);
//         glm_normalize(mul);
//         glm_vec3_scale(mul, state.camera.speed*state.window.delta_time, mul);
//         glm_vec3_add(state.camera.pos, mul, state.camera.pos);
//     }
//     if (glfwGetKey(window, 'O') == GLFW_PRESS)
//         state.camera.ortho = 1;
//     if (glfwGetKey(window, 'P') == GLFW_PRESS)
//         state.camera.ortho = 0;
//     printf("not implemented\n");
// }