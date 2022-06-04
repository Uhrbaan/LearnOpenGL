#include "input.h"
#include "camera.h"
#include "utils/utils.h"

struct camera cam; // informer de l'existence du extern
float delta_time;
float last_frame;

static float sensitivity = 0.1f;
static bool first_mouse = true;
float cursor_lastx, cursor_lasty;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
        vec3 mul; glm_vec3_scale(cam.z, cam.speed*delta_time, mul);
        glm_vec3_add(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        vec3 mul; glm_vec3_scale(cam.z, cam.speed*delta_time, mul);
        glm_vec3_sub(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(cam.z, cam.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, cam.speed*delta_time, mul);
        glm_vec3_sub(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        vec3 mul; 
        glm_cross(cam.z, cam.y, mul);
        glm_normalize(mul);
        glm_vec3_scale(mul, cam.speed*delta_time, mul);
        glm_vec3_add(cam.pos, mul, cam.pos);
    }
    if (glfwGetKey(window, 'O') == GLFW_PRESS)
        cam.ortho = 1;
    if (glfwGetKey(window, 'P') == GLFW_PRESS)
        cam.ortho = 0;
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
        // quoi faire en quand la fenêtre change de taille
        glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
        // actions de la soursi
        glfwSetCursorPosCallback(*window, mouse_callback);
        // scroll
        glfwSetScrollCallback(*window, scroll_callback);
        
        return 0;
    }
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // callback function that tells what to do on a resize
    cam.w = width;
    cam.h = height;
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double x, double y)
{

    // if (unlikely(first_mouse))
    // {
    //     cursor_lastx = x;
    //     cursor_lasty = y;
    //     first_mouse = false;
    // }

    float offx = (x-cursor_lastx)*sensitivity;
    float offy = (y-cursor_lasty)*sensitivity;
    cursor_lastx = x;
    cursor_lasty = y;

    cam.yaw += offx;
    cam.pitch -= offy;

    // to prevent the lookat function to flip the camera, we need to prevent the 
    // user to have an angle greater than 89°
    if(cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    else if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cam.fov -= (float)yoffset*sensitivity*5;
        if (cam.fov < 1.0f)
            cam.fov = 1.0f;
        else if (cam.fov > 80.0f)
            cam.fov = 80.0f;
    }
    else
    {
        cam.speed += yoffset*sensitivity*2;
    }
}
