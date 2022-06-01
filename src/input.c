#include "input.h"
#include "camera.h"
#include "utils/utils.h"

struct camera cam; // informer de l'existence du extern

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, 'I') == GLFW_PRESS) // !! la lettre doit être maj
        // set wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, 'F') == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // movement
    if (glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        vec3 mul; glm_vec3_scale(cam.front, cam.speed*delta_time, mul);
        glm_vec3_add(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        vec3 mul; glm_vec3_scale(cam.front, cam.speed*delta_time, mul);
        glm_vec3_sub(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(cam.front, cam.up, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, cam.speed*delta_time, mul);
        glm_vec3_sub(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(cam.front, cam.up, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, cam.speed*delta_time, mul);
        glm_vec3_add(cam.pos, mul, cam.pos);
    }
}
int initGLFW(GLFWwindow **window, int width, int height, 
             const char* title, GLFWmonitor* monitor, 
             GLFWwindow* share)
{
    if (likely(glfwInit() == GLFW_TRUE))
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        *window = glfwCreateWindow(width, height, title, monitor, share);
        if (unlikely(*window == NULL))
        {
            printf(TERM_COL_ERROR("error") ":Failed to create GLFW window\n");
            glfwTerminate();
            return 1;
        }
        glfwMakeContextCurrent(*window);
        glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
            // quoi faire en quand la fenêtre change de taille
        
        return 0;
    }
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // callback function that tells what to do on a resize
    glViewport(0, 0, width, height);
}