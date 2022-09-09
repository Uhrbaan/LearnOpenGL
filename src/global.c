#include "global.h"
#include "game/input/input.h"
#include "render/shading.h"
#include "window/window.h"

struct game_data state;

void initGlobalState(int window_w, int window_h, char *title)
{
    void *getProcAdress_fn;
    state.window = initWindow(window_w, window_h, title, &getProcAdress_fn);
    initOpenGL(0, 0, window_w, window_h, getProcAdress_fn);
    // state.camera = initCamera;
}

void updateCameraUniform(struct camera camera, unsigned int shader_program);
void linkCamera2Uniform(struct camera *camera, unsigned int shader_program);

static float last_frame = 0;
int main_loop(unsigned int shader_program, struct model model)
{
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(state.window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    linkCamera2Uniform(&state.camera, shader_program);

    while(!glfwWindowShouldClose(state.window.glfw_window))
    {
        float current_frame = glfwGetTime();
        state.delta_time    = current_frame - last_frame;
        last_frame          = current_frame;

        movement(state.window.glfw_window);

        updateCamera(&state.camera, shader_program);
        glm_vec3_copy(state.camera.pos, spot_light[0].position);
        glm_vec3_copy(state.camera.z,   spot_light[0].direction);
        updatespotLight(0, shader_program);

        updateCameraUniform(state.camera, shader_program);

        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawModel(model, shader_program); 

        glfwSwapBuffers(state.window.glfw_window);
        glfwPollEvents();
        msleep(16);
    }
    glfwTerminate();
    return 0;
}

static struct temp_cam_uni
{
    unsigned int model, view, projection;
} cam = {0};
void updateCameraUniform(struct camera camera, unsigned int shader_program)
{
    glUseProgram(shader_program);
    glUniformMatrix4fv(cam.model, 1, 0,
       (const float *) camera.model);
    glUniformMatrix4fv(cam.view,  1, 0,
       (const float *) camera.view);
    glUniformMatrix4fv(cam.projection, 1, 0,
       (const float *) camera.projection);
}

void linkCamera2Uniform(struct camera *camera, unsigned int shader_program)
{
    cam.view =
        glGetUniformLocation(shader_program, "view");
    cam.model =
        glGetUniformLocation(shader_program, "model");
    cam.projection =
        glGetUniformLocation(shader_program, "projection");
}