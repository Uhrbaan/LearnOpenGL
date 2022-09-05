#include "opengl.h"
#include "../utils/utils.h"
#include <assert.h>
#include "shading.h"
// #include <glad/gl.h>

void GLAPIENTRY
MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity,
                 GLsizei length, const GLchar* message, const void* userParam )
{
  fprintf(stderr, "%s type = 0x%x, severity = 0x%x, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? TERM_COL_ERROR("GL error: ") : "???" ),
        type, severity, message );
}

int initGlad(int x, int y, int w, int h, void* fn_proc_adress)
{
    if (!gladLoadGLLoader((GLADloadproc) fn_proc_adress))
    {
        printf(TERM_COL_ERROR("error") ": Failed to initialize GLAD\n");
        return 1;
    }
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
    glViewport(x, y, w, h);
    return 0;
}

unsigned int loadShader(const char *path, unsigned int type)
{
    int success;
    unsigned int shader; // id du shader
    const char *shader_source;
    
    shader_source = file2str(path);
    if (!shader_source)
        return 0; 
    shader = glCreateShader(type); // crée un id pour le shader

    // attaching shader source to id and compiling
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // checking for compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
        exit(EXIT_FAILURE);
    free((char*)shader_source); // casting to non-const
    return shader;
}

unsigned int createShaderProgram(unsigned int vs, unsigned int fs)
{
    unsigned int program=0; int linked=0;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char info_log[512];
        glGetShaderInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, ERROR_MSG_LOG("failed shader linking", info_log));
        program = 0;
    }
    // freeing shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

inline unsigned int loadShaderProgram(const char *vs_path, const char *fs_path)
{
    return createShaderProgram(
        loadShader(vs_path, GL_VERTEX_SHADER),
        loadShader(fs_path, GL_FRAGMENT_SHADER)
    );
}

#include <stdbool.h>
#include "stb_image.h"
unsigned int loadGLTexture(const char *path) // TODO make function faster
{
    printf("loading texture %s...\n", path);
    stbi_set_flip_vertically_on_load(true);
    // loading texture
    int w, h, channels;
    unsigned int texture;
    unsigned char *data = NULL;
    data = stbi_load(path, &w, &h, &channels, 0);
    if (!data)
    {
        fprintf(stderr, TERM_COL_ERROR("error: ") 
                        TERM_COL_INFO("Failed to load texture:")
                        "%s\n", path);
        return 0;
    }
    
    // generating gl texture
    unsigned int format=0;
    switch (channels)
    {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, TERM_COL_ERROR("error:")
                            TERM_COL_INFO("color format is invalid:") 
                    "%d number of channels", channels);
            break;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,      // texture type
        0,                  // manual mipmap level
        format,             // stored color format
        w, h, 
        0,                  // legacy
        format,             // color format of the texture
        GL_UNSIGNED_BYTE,
        data                // img data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind
    free(data);

    return texture;
}

// TODO optimise to not search uniform location everytime
void useTexture(int index, char *uniform_name, unsigned int shader_program, 
                unsigned int gl_id)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glUseProgram(shader_program);
    unsigned int u_loc = glGetUniformLocation(shader_program, uniform_name);
    glUniform1i(u_loc, index); // link to n-th sampler
    glBindTexture(GL_TEXTURE_2D, gl_id);
}

void drawElements(unsigned int shader_program, unsigned int vao, int n_indices)
{
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

unsigned int genVao(unsigned int *vbo, unsigned int *ebo, 
                    void *vertices, size_t vert_sz, 
                    void *indices, size_t indi_sz, size_t stride)
{
    unsigned int _vao, _vbo, _ebo;
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_sz, vertices, GL_STATIC_DRAW);

    glBindVertexArray(_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indi_sz, indices, GL_STATIC_DRAW);
    
    size_t off_norm = 3*sizeof(float), off_uv= 6*sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);          // location
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)off_norm);          // normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)off_uv);            // texture coordinates
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // unbind current vao
    *vbo = _vbo;
    *ebo = _ebo;
    return _vao;
}