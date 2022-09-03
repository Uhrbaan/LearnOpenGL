#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "opengl.h"
#include "shading.h"

unsigned int material_uniform= 0;

struct directional_light *directional_light = NULL;
unsigned int directional_light_n = 0;
static struct 
{ 
    unsigned int direction, ambient, diffuse, specular; 
} *directional_light_uniform = NULL;
    
struct point_light *point_light = NULL;
unsigned int point_light_n = 0;
static struct 
{
    unsigned int position, ambient, diffuse, specular, constant, 
                 linear, quadratic;                             
} *point_light_uniform = NULL;

struct spot_light *spot_light = NULL;
unsigned int spot_light_n = 0;
static struct 
{ 
    unsigned int position, direction, ambient, diffuse, specular,
                 constant, linear, quadratic, cutoff, outer_cutoff;
} *spot_light_uniform = NULL;


static unsigned int LIGHT_SHADER_PROGRAM;


void initShading(unsigned int shader_program)
{
    int i; char uniform_name[100];
    LIGHT_SHADER_PROGRAM = shader_program;
    // find the number of each type of light
    material_uniform = glGetUniformLocation(shader_program, "material");


    glGetUniformuiv(                                                             // directional light
        shader_program, 
        glGetUniformLocation(shader_program, "directional_light_n"),
        &directional_light_n);

    directional_light_uniform =
        calloc(directional_light_n, sizeof(*directional_light_uniform));
    assert(directional_light_uniform);

    for (i=0; i<directional_light_n; i++)
    {
        snprintf(uniform_name, 99, "directional_light[%d].direction", i);
        directional_light_uniform[i].direction = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].ambient", i);
        directional_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].diffuse", i);
        directional_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "directional_light[%d].specular", i);
        directional_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
    }

    directional_light = 
        calloc(directional_light_n, sizeof(struct directional_light));
    assert(directional_light);


    glGetUniformuiv(                                                             // point light
        shader_program, 
        glGetUniformLocation(shader_program, "point_light_n"),
        &point_light_n);

    point_light_uniform = 
        calloc(point_light_n, sizeof(*point_light_uniform));
    assert(point_light_uniform);

    for (i=0; i<point_light_n; i++)
    {
        snprintf(uniform_name, 99, "point_light[%d].position", i);
        point_light_uniform[i].position = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].ambient", i);
        point_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].diffuse", i);
        point_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].specular", i);
        point_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].constant", i);
        point_light_uniform[i].constant = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].linear", i);
        point_light_uniform[i].linear = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 99, "point_light[%d].quadratic", i);
        point_light_uniform[i].quadratic = 
            glGetUniformLocation(shader_program, uniform_name);
    }

    point_light = 
        calloc(point_light_n, sizeof(struct point_light));
    assert(point_light);


    glGetUniformuiv(                                                             // spot light
        shader_program, 
        glGetUniformLocation(shader_program, "spot_light_n"),
        &spot_light_n);

    spot_light_uniform = 
        calloc(spot_light_n, sizeof(*spot_light_uniform));
    assert(point_light_uniform);

    for (i=0; i<spot_light_n; i++)
    {
        snprintf(uniform_name, 100-1, "spot_light[%d].position", i);
        spot_light_uniform[i].position = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].direction", i);
        spot_light_uniform[i].direction = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].ambient", i);
        spot_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].diffuse", i);
        spot_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].specular", i);
        spot_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].constant", i);
        spot_light_uniform[i].constant = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].linear", i);
        spot_light_uniform[i].linear = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].quadratic", i);
        spot_light_uniform[i].quadratic = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].cutoff", i);
        spot_light_uniform[i].cutoff = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100-1, "spot_light[%d].outer_cutoff", i);
        spot_light_uniform[i].outer_cutoff = 
            glGetUniformLocation(shader_program, uniform_name);
    }

    spot_light = 
        calloc(spot_light_n, sizeof(struct spot_light));
    assert(spot_light);
}

void updateDirectionalLight(unsigned int index)
{
    glUseProgram(LIGHT_SHADER_PROGRAM);

    glUniform3fv(directional_light_uniform[index].diffuse, 1, 
                 directional_light[index].diffuse);
    glUniform3fv(directional_light_uniform[index].ambient, 1, 
                 directional_light[index].ambient);
    glUniform3fv(directional_light_uniform[index].diffuse, 1, 
                 directional_light[index].diffuse);
    glUniform3fv(directional_light_uniform[index].specular, 1, 
                 directional_light[index].specular);
}

void updatePointLight(unsigned int index)
{
    glUseProgram(LIGHT_SHADER_PROGRAM);

    glUniform3fv(point_light_uniform[index].diffuse, 1, 
                 point_light[index].diffuse);
    glUniform3fv(point_light_uniform[index].ambient, 1, 
                 point_light[index].ambient);
    glUniform3fv(point_light_uniform[index].diffuse, 1, 
                 point_light[index].diffuse);
    glUniform3fv(point_light_uniform[index].specular, 1, 
                 point_light[index].specular);
    glUniform1f (point_light_uniform[index].constant,
                 point_light[index].constant);
    glUniform1f (point_light_uniform[index].linear,
                 point_light[index].linear);
    glUniform1f (point_light_uniform[index].quadratic,
                 point_light[index].quadratic);
}
void updatespotLight(unsigned int index)
{
    glUseProgram(LIGHT_SHADER_PROGRAM);

    glUniform3fv(spot_light_uniform[index].diffuse, 1, 
                 spot_light[index].diffuse);
    glUniform3fv(spot_light_uniform[index].ambient, 1, 
                 spot_light[index].ambient);
    glUniform3fv(spot_light_uniform[index].diffuse, 1, 
                 spot_light[index].diffuse);
    glUniform3fv(spot_light_uniform[index].specular, 1, 
                 spot_light[index].specular);
    glUniform1f (spot_light_uniform[index].constant,
                 spot_light[index].constant);
    glUniform1f (spot_light_uniform[index].linear,
                 spot_light[index].linear);
    glUniform1f (spot_light_uniform[index].quadratic,
                 spot_light[index].quadratic);
    glUniform1f (spot_light_uniform[index].cutoff,
                 spot_light[index].cutoff);
    glUniform1f (spot_light_uniform[index].outer_cutoff,
                 spot_light[index].outer_cutoff);
}