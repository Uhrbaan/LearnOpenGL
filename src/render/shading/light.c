#include <string.h>
#include <assert.h>

#include "light.h"
#include "../opengl.h"
#include "../../utils/texture_types.h"

struct directional_light_uniform
{ 
    unsigned int ambient, diffuse, specular,
                 direction;
};
struct point_light_uniform
{
    unsigned int ambient, diffuse, specular, 
                 position, constant, linear, quadratic;                             
};
struct spot_light_uniform
{ 
    unsigned int ambient, diffuse, specular,
                 position, direction, constant, linear, quadratic, cutoff, outer_cutoff;
};

static struct directional_light_uniform directional_light_uniform[MAX_DIRECTIONAL_LIGHTS] = {0};
static struct point_light_uniform       point_light_uniform[MAX_POINT_LIGHTS]       = {0};
static struct spot_light_uniform        spot_light_uniform[MAX_SPOT_LIGHTS]        = {0};
struct directional_light directional_light[MAX_DIRECTIONAL_LIGHTS] = {0};
struct point_light       point_light      [MAX_POINT_LIGHTS]       = {0};
struct spot_light        spot_light       [MAX_SPOT_LIGHTS]        = {0};
unsigned int directional_light_n = 0;
unsigned int point_light_n       = 0;
unsigned int spot_light_n        = 0;

// TODO refactor
void initLights (unsigned int shader_program)
{

    int i; char uniform_name[100];
    // find the number of each type of light
    for (i=0; i<MAX_DIRECTIONAL_LIGHTS; i++)
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
    for (i=0; i<MAX_DIRECTIONAL_LIGHTS; i++) updateDirectionalLight(i, shader_program);

    for (i=0; i<MAX_POINT_LIGHTS; i++)
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
    for (i=0; i<MAX_POINT_LIGHTS; i++) updatePointLight(i, shader_program);

    for (i=0; i<MAX_SPOT_LIGHTS; i++)
    {
        snprintf(uniform_name, 100, "spot_light[%d].position", i);
        spot_light_uniform[i].position = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].direction", i);
        spot_light_uniform[i].direction = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].ambient", i);
        spot_light_uniform[i].ambient = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].diffuse", i);
        spot_light_uniform[i].diffuse = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].specular", i);
        spot_light_uniform[i].specular = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].constant", i);
        spot_light_uniform[i].constant = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].linear", i);
        spot_light_uniform[i].linear = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].quadratic", i);
        spot_light_uniform[i].quadratic = 
            glGetUniformLocation(shader_program, uniform_name);
        snprintf(uniform_name, 100, "spot_light[%d].cutoff", i);
        spot_light_uniform[i].cutoff = 
            glGetUniformLocation(shader_program, uniform_name);
    }
    for (i=0; i<directional_light_n; i++) updateSpotLight(i, shader_program);
}

/**************************** Update Lights ***********************************/

void updateDirectionalLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(directional_light_uniform[index].direction, 1, 
                 directional_light[index].direction);
    glUniform3fv(directional_light_uniform[index].ambient, 1, 
                 directional_light[index].ambient);
    glUniform3fv(directional_light_uniform[index].diffuse, 1, 
                 directional_light[index].diffuse);
    glUniform3fv(directional_light_uniform[index].specular, 1, 
                 directional_light[index].specular);
}

void updatePointLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(point_light_uniform[index].position, 1, 
                 point_light[index].position);
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
void updateSpotLight(unsigned int index, unsigned int shader_program)
{
    glUseProgram(shader_program);

    glUniform3fv(spot_light_uniform[index].position, 1, 
                 spot_light[index].position);
    glUniform3fv(spot_light_uniform[index].direction, 1, 
                 spot_light[index].direction);
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

/***************************** Create lights **********************************/

void createDirectionalLight(int index, 
                            vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular)
{
    glm_vec3_copy(direction,directional_light[index].direction);
    glm_vec3_copy(ambient,  directional_light[index].ambient);
    glm_vec3_copy(diffuse,  directional_light[index].diffuse);
    glm_vec3_copy(specular, directional_light[index].specular);
}

void createPointLight(int index, 
                      vec3 pos, vec3 ambient, vec3 diffuse, vec3 specular,
                      float linear, float quadratic)
{
    glm_vec3_copy(pos,      point_light[index].position);
    glm_vec3_copy(ambient,  point_light[index].ambient);
    glm_vec3_copy(diffuse,  point_light[index].diffuse);
    glm_vec3_copy(specular, point_light[index].specular);
    point_light[index].constant  = 1.0f;
    point_light[index].linear    = linear;
    point_light[index].quadratic = quadratic;
}

void createSpotLight(int index, 
                     vec3 pos, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular,
                     float linear, float quadratic,
                     float cutoff_deg, float outer_cutoff_deg)
{
    glm_vec3_copy(pos,       spot_light[index].position);
    glm_vec3_copy(direction, spot_light[index].direction);
    glm_vec3_copy(ambient,   spot_light[index].ambient);
    glm_vec3_copy(diffuse,   spot_light[index].diffuse);
    glm_vec3_copy(specular,  spot_light[index].specular);
    spot_light[index].constant  = 1.0f;
    spot_light[index].linear    = linear;
    spot_light[index].quadratic = quadratic;
    spot_light[index].cutoff       = cos(glm_rad(cutoff_deg));
    spot_light[index].outer_cutoff = cos(glm_rad(outer_cutoff_deg));
}