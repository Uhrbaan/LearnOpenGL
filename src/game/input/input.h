#ifndef INPUT_H
#define INPUT_H

enum input_context
{
    general,
    navigation,
    text_input
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void movement(GLFWwindow *window);

#endif