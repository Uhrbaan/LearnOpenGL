#include <stdio.h>
#include <stdbool.h>

#include "gl.h"
#include "utils.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // callback function that tells what to do on a resize
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, 'W') == GLFW_PRESS) // !! la lettre doit être maj
        // set wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, 'F') == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int getglError(unsigned int object, GLenum type, const char* info_message)
{
    int success; char info_log[512];
    glGetShaderiv(object, type, &success);
    if (unlikely(!success))
    {
        glGetShaderInfoLog(object, 512, NULL, info_log);
        fprintf(stderr, TERM_COL_ERROR("error")
                        ": " 
                        TERM_COL_INFO("%s") 
                        ": %s\n", info_message, info_log);
        return 1;
    }
    return 0;
}

unsigned int FILE2shader(const char *file_path, GLenum shader_type)
{
    unsigned int shader; // id du shader
    const char *shader_source;
    
    shader_source = FILE2str(file_path);
    if (!shader_source)
        return 0; 
    shader = glCreateShader(shader_type); // crée un id pour le shader

    // attaching shader source to id and compiling
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // checking for compile errors
    if (getglError(shader, GL_COMPILE_STATUS, "failed shader comilation"))
        return 0;

    return shader;
}

unsigned int linkShaders2program(int n, ...)
{
    va_list valist;
    unsigned int shader_program = glCreateProgram(), i;
    
    va_start(valist, n);
    for (i=0; i<n; i++)
        glAttachShader(shader_program, va_arg(valist, unsigned int));
    glLinkProgram(shader_program);

    if (getglError(shader_program, GL_LINK_STATUS, 
                   "failed to link shaders into program"))
        return 0;

    // deleting shaders, as we dont need them anymore
    va_start(valist, n);
    for (i=0; i<n; i++)
        glDeleteShader(va_arg(valist, unsigned int));
    va_end(valist);

    return shader_program;
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
            printf("%d, %d, %d, %d\n", va_arg(valist, int),
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

unsigned int FILE2texture(const char *img_path, GLenum texture_type)
{
    // loading texture
    int w, h, n_channels;
    unsigned char *data = stbi_load("res/textures/container.jpg",
                                    &w, &h, &n_channels, 0);
    if (!data)
    {
        printf(TERM_COL_ERROR("error") ": Failed to load texture\n");
        return 0;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(texture_type, texture);
    // loading image in texture
    glTexImage2D(texture_type,     // modifying current boudn TEXTURE2D
                 0,                 // level of mipmap if made manualy
                 GL_RGB,            // format in which the texture is stored
                 w,                 // width
                 h,                 // height
                 0,                 // always 0 (because legacy stuff)
                 GL_RGB,            // format & data type of the texture 
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