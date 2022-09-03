#include "global.h"
#include "game/input/input.h"

struct game_data state;
void linkCamera2Uniform(struct camera *camera, unsigned int shader_program);
void updateCameraUniform(struct camera camera, unsigned int shader_program);

void initApplicationGlobalState(int win_w, int win_h, char *title)
{
    // window
    void * result_fn = NULL;
    state.window.w = win_w;
    state.window.h = win_h;
    strncpy(state.window.title, title, 199);
    result_fn = 
        initGLFW(&state.window.glfw_window, win_w, win_h, title, NULL, NULL);
    assert(result_fn);
    initGlad(0, 0, win_w, win_h, result_fn);
}

void initCamera(bool ortho_projection, vec3 cam_pos, 
                vec3 cam_x, vec3 cam_y, vec3 cam_z,
                float fov, float yaw, float pitch, float roll,
                unsigned int shader_program)
{
    state.camera.ortho = ortho_projection;                                      // generate camera fields
    state.camera.ratio = (float)state.window.w/(float)state.window.h;
    memcpy(state.camera.pos, cam_pos, sizeof(vec3));
    memcpy(state.camera.x, cam_x, sizeof(vec3));
    memcpy(state.camera.y, cam_y, sizeof(vec3));
    memcpy(state.camera.z, cam_z, sizeof(vec3));
    state.camera.fov = fov;
    state.camera.yaw = yaw; 
    state.camera.pitch = pitch;
    state.camera.roll = roll; // will probably not be used
    updateCamera(&state.camera);
    linkCamera2Uniform(&state.camera, shader_program);                          // link the camera struc to the according
    updateCameraUniform(state.camera, shader_program);                          // uniform in the shader
    glm_mat4_identity(state.camera.model);
}

void updateCameraUniform(struct camera camera, unsigned int shader_program)
{
    glUseProgram(shader_program);
    glUniformMatrix4fv(camera.uni_loc.model, 1, 0, 
       (const float *) camera.model);
    glUniformMatrix4fv(camera.uni_loc.view,  1, 0, 
       (const float *) camera.view);
    glUniformMatrix4fv(camera.uni_loc.projection, 1, 0, 
       (const float *) camera.projection);
}

void linkCamera2Uniform(struct camera *camera, unsigned int shader_program)
{
    camera->uni_loc.view = 
        glGetUniformLocation(shader_program, "view");
    camera->uni_loc.model = 
        glGetUniformLocation(shader_program, "model");
    camera->uni_loc.projection = 
        glGetUniformLocation(shader_program, "projection");
}

static float last_frame = 0;
int main_loop(unsigned int shader_program, struct model model)
{
    glEnable(GL_DEPTH_TEST);                                                    // initialisation
    // disable cursor
    glfwSetInputMode(state.window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while(!glfwWindowShouldClose(state.window.glfw_window))
    {
        // uptdate delta
        float current_frame = glfwGetTime();
        state.delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        movement(state.window.glfw_window);

        // clearing screen
        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering
        drawModel(model, shader_program);

        // processing perspective changes
        updateCamera(&state.camera);
        updateCameraUniform(state.camera, shader_program);
        glUniform3fv(
            glGetUniformLocation(shader_program, "cam_pos"),
            1, state.camera.pos
        );

        glfwSwapBuffers(state.window.glfw_window);

        glfwPollEvents();
        msleep(16);
    }
    glfwTerminate();
    return 0;
}

void generateVAO(struct mesh *mesh)
{
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->n_vert * sizeof (struct vertex),
                 mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_indi * sizeof (unsigned int),
                 mesh->indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), 
                          (void*)0);                // location
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), 
                          (void*)(3*sizeof(float)));// normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (void*)(6*sizeof(float)));// texture coordinates
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // unbind current vao

    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
}

// drawing
void drawMesh(struct mesh m, unsigned int shader_program)
{
    int diffuse_n=0, specular_n=0, n=0;
    char uniform_name[100] = {0};
    char base[20] = "texture_";
    char type[20] = {0};

    for (int i=0; i<m.n_text; i++)
    {
        switch (m.textures[i]->type)
        {
            case diffuse:
                n = diffuse_n++;
                strncpy(type, "diffuse", 20-1);
                break;
            case specular:
                n = specular_n++;
                strncpy(type, "specular", 20-1);
            default:
                break;
        }
        snprintf(uniform_name, 100-1, "%s%s[%d]", base, type, n);
        useTexture(i, uniform_name, shader_program, m.textures[i]->gl_id);
    }

    drawElements(shader_program, m.vao, m.n_indi);
}

void drawModel(struct model m, unsigned int shader_program)
{
    for (int i=0; i<m.n_meshes; i++)
        drawMesh(m.meshes[i], shader_program);
}