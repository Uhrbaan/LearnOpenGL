#include "window.h"
#include <cglm/cglm.h>
#include <string.h>
#include "../game/input/input.h"
#include "../utils/utils.h"

void glfwErrorCallback(int error_code, const char* description)
{
    fprintf(stderr, 
            TERM_COL_ERROR("error: ")TERM_COL_INFO("%s")" error code: %d",
            description, error_code);
}

struct window initWindow(int window_w, int window_h, char *title, void **fn)
{
    if (!glfwInit()) exit(EXIT_FAILURE);
    
    struct window window = {0};
    window.w = window_w;
    window.h = window_h;
    strncpy(window.title, title, 200);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.glfw_window = 
        glfwCreateWindow(window_w, window_h, title, NULL, NULL);
    if (window.glfw_window == NULL)
    {
        fprintf(stderr, TERM_COL_ERROR("error:") 
                "failded to create glfw window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window.glfw_window);                                  // callbacks for window
    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetErrorCallback           (glfwErrorCallback);  
    glfwSetFramebufferSizeCallback (window.glfw_window, framebuffer_size_callback);
    glfwSetCursorPosCallback       (window.glfw_window, mouse_callback);
    glfwSetScrollCallback          (window.glfw_window, scroll_callback);
    glfwSetKeyCallback             (window.glfw_window, key_callback);
        
    *fn = glfwGetProcAddress;
    return window;
}