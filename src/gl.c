#include "gl.h"
#include <stdio.h>
#include <stdbool.h>
#include <cglm/cglm.h>

#include "utils/utils.h"
#include "utils/stb_image.h"

int initGLAD(GLint x, GLint y, GLsizei width, GLsizei height)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf(TERM_COL_ERROR("error") ": Failed to initialize GLAD\n");
        return 1;
    }
    glViewport(x, y, width, height);
    return 0;
}

unsigned int file2shader(const char *file_path, GLenum shader_type)
{
    int success;
    unsigned int shader; // id du shader
    const char *shader_source;
    
    shader_source = file2str(file_path);
    if (!shader_source)
        return 0; 
    shader = glCreateShader(shader_type); // crée un id pour le shader

    // attaching shader source to id and compiling
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // checking for compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, ERROR_MSG_LOG("failed shader compilation", info_log));
        shader = 0;
    }
    free((char*)shader_source); // casting to non-const
    return shader;
}

unsigned int createShaderProgram(const char *vertex_shader, 
                                 const char *fragment_shader)
{
    unsigned int program=0, vs=0, fs=0;
    int linked=0;
    vs = file2shader(vertex_shader, GL_VERTEX_SHADER);
    fs = file2shader(fragment_shader, GL_FRAGMENT_SHADER);
    if (fs && vs)
    {
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
    const char *s=NULL;
    if (vs)
        s=vertex_shader;
    else
        s=fragment_shader;
    fprintf(stderr, ERROR_MSG_LOG("one of the shaders is invalid", s));
    return 0;
}

unsigned int setUniform(unsigned int shader_program, int type,
                        const char *name, ...)
{
    va_list valist;
    int uni_location = glGetUniformLocation(shader_program, name);
    switch (type)
    {
        // floats & doubles -> because va_arg promotes to float to double
        case GL_FLOAT: case GL_DOUBLE:
            va_start(valist, name);
            glUniform1f(uni_location, va_arg(valist, double));
            break;

        case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:
            va_start(valist, name);
            glUniform2f(uni_location, va_arg(valist, double), 
                                      va_arg(valist, double));
            break;
        
        case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:
            va_start(valist, name);
            glUniform3f(uni_location, va_arg(valist, double), 
                                      va_arg(valist, double), 
                                      va_arg(valist, double));
            break;

        case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:
            va_start(valist, name);
            glUniform4f(uni_location, va_arg(valist, double), 
                                      va_arg(valist, double), 
                                      va_arg(valist, double),
                                      va_arg(valist, double));
            break;

        // ints & bool
        case GL_INT: case GL_BOOL:
            va_start(valist, name);
            glUniform1i(uni_location, va_arg(valist, int));
            break;

        case GL_INT_VEC2: case GL_BOOL_VEC2:
            va_start(valist, name);
            glUniform2i(uni_location, va_arg(valist, int),
                                      va_arg(valist, int));
            break;
        
        case GL_INT_VEC3: case GL_BOOL_VEC3:
            va_start(valist, name);
            glUniform3i(uni_location, va_arg(valist, int), 
                                      va_arg(valist, int), 
                                      va_arg(valist, int));
            break;

        case GL_INT_VEC4: case GL_BOOL_VEC4:
            va_start(valist, name);
            glUniform4i(uni_location, va_arg(valist, int),
                                      va_arg(valist, int),
                                      va_arg(valist, int),
                                      va_arg(valist, int));

            break;

        // uint
        case GL_UNSIGNED_INT:
            va_start(valist, name);
            glUniform1ui(uni_location, va_arg(valist, unsigned int));
            break;

        case GL_UNSIGNED_INT_VEC2:
            va_start(valist, name);
            glUniform2ui(uni_location, va_arg(valist, unsigned int),
                                       va_arg(valist, unsigned int));
            break;
        
        case GL_UNSIGNED_INT_VEC3:
            va_start(valist, name);
            glUniform3ui(uni_location, va_arg(valist, unsigned int), 
                                       va_arg(valist, unsigned int), 
                                       va_arg(valist, unsigned int));
            break;

        case GL_UNSIGNED_INT_VEC4:
            va_start(valist, name);
            glUniform4ui(uni_location, va_arg(valist, unsigned int), 
                                       va_arg(valist, unsigned int), 
                                       va_arg(valist, unsigned int),
                                       va_arg(valist, unsigned int));
            break;
        
        default:
            break;
    }
    va_end(valist);
    return uni_location;
}

// TODO gert rid of color format -> use n_channels
unsigned int file2texture(const char *img_path, GLenum color_format, 
                          GLenum texture_type)
{
    stbi_set_flip_vertically_on_load(true);
    // loading texture
    int w, h, n_channels;
    unsigned int texture;
    unsigned char *data = NULL;
    data = stbi_load(img_path, &w, &h, &n_channels, 0);
    if (!data)
    {
        printf(TERM_COL_ERROR("error: ") TERM_COL_INFO("Failed to load texture")
                                                       "\n");
        return 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(texture_type, texture);

    // loading image in texture
    glTexImage2D(texture_type,      // modifying current boudn TEXTURE2D
                 0,                 // level of mipmap if made manualy
                 color_format,            // format in which the texture is stored
                 w,                 // width
                 h,                 // height
                 0,                 // always 0 (legacy stuff)
                 color_format,      // format & data type of the texture 
                 GL_UNSIGNED_BYTE,  // └> rgb / bytes (char)
                 data               // image data
                 );
    glGenerateMipmap(texture_type);
    stbi_image_free(data); // free image data from memory
    return texture;
}

void setTextureParam(int n, unsigned int texture, GLenum texture_type, ...)
{
    /* modifie l'apparence de la texture si les coordonnées ne map pas l'objet
     * options : GL_REPEAT (défaut), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, 
     *           GL_CLAMP_TO_BORDER ||  if clamp to edge is chosen:
     *  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
     *  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/
    /* texture filtering: ce que doit faire opengl quand les texels n'alignent
     * pas les pixels : GL_NEAREST, GL_LINEAR 
     * can be set on magnifying (GL_TEXTURE_MAG_FILTER) or minifying
     * (GL_TEXTURE_MIN_FILTER) operations */
    /* to prevent memory wastage, when small or far objects are rendered, smaler
     * textures are used (mipmaps). can be generated through glGenerateMipmaps
     * p. 58 */
    va_list valist;
    glBindTexture(texture_type, texture);
    va_start(valist, texture_type);
    for (int i=0; i<n; i++)
    {
        glTexParameteri(texture_type, va_arg(valist, GLenum), 
                        va_arg(valist, GLenum));
    }
}

mat4wloc createUniformMatrix(const char *uniform_name, unsigned int n_programs,
                             unsigned int *shader_programs)
{
    mat4wloc um4 = {0};
    glm_mat4_identity(um4.m);
    um4.n_programs = n_programs;
    um4.shader_programs = malloc(n_programs*sizeof(unsigned int));
    um4.uniform_locations = malloc(n_programs*sizeof(unsigned int));
    for (int i=0; i<n_programs; i++)
    {
        um4.shader_programs[i] = shader_programs[i];
        um4.uniform_locations[i] = 
            glGetUniformLocation(shader_programs[i], uniform_name);
    }
    return um4;
}
void updateUniformMatrix(mat4wloc um4, int transpose)
{
    for (int i=0; i<um4.n_programs; i++)
    {
        glUseProgram(um4.shader_programs[i]);
        glUniformMatrix4fv(um4.uniform_locations[i], 1, transpose, (float*)(um4.m));
    }
}