#include "lighting.h"
#include "gl.h"
#include <string.h>

void updateMaterial(Material material)
{
    glUseProgram(material.shader_program);
    setUniform(material.shader_program, GL_FLOAT, "material.shininess", 
               material.shininess);
    setUniform(material.shader_program, GL_INT, "material.diffuse", 0);
    setUniform(material.shader_program, GL_INT, "material.specular", 1);
    setUniform(material.shader_program, GL_INT, "material.emission", 2);
}

void updateLight(void* p, const char *uniform_name, enum EnumLIGHT type)
{
    char str[100]={0}; int len=strlen(uniform_name);
    memcpy(str, uniform_name, sizeof(char)*len);
    str[len] = '.';
    switch (type)
    {
        case DIRECTIONAL:
        {
            Directional_light *l = p;
            unsigned int arr[4];
            glUseProgram(l->shader_program);
            memcpy(&str[len+1], "direction", sizeof "direction");
            arr[0] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "ambient", sizeof "ambient");
            arr[1] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "diffuse", sizeof "diffuse");
            arr[2] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "specular", sizeof "specular");
            arr[3] = glGetUniformLocation(l->shader_program, str);
            glUniform3fv(arr[0], 1, l->direction);
            glUniform3fv(arr[1], 1, l->ambient);
            glUniform3fv(arr[2], 1, l->diffuse);
            glUniform3fv(arr[3], 1, l->specular);
            memcpy(l->arr, arr, sizeof(arr));
            break;
        }
        case POINT:
        {
            Point_light *l = p;
            glUseProgram(l->shader_program);
            unsigned int arr[7];
            memcpy(&str[len+1], "position", sizeof "position");
            arr[0]=glGetUniformLocation(l->shader_program, str);

            memcpy(&str[len+1], "ambient", sizeof "ambient");
            arr[1] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "diffuse", sizeof "ambient");
            arr[2] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "specular", sizeof "specular");
            arr[3]=glGetUniformLocation(l->shader_program, str);

            memcpy(&str[len+1], "constant", sizeof "constant");
            arr[4]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "linear", sizeof "linear");
            arr[5]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "quadratic", sizeof "quadratic");
            arr[6]=glGetUniformLocation(l->shader_program, str);

            glUniform3fv(arr[0], 1, l->position);
            glUniform3fv(arr[1], 1, l->ambient);
            glUniform3fv(arr[2], 1, l->diffuse);
            glUniform3fv(arr[3], 1, l->specular);

            glUniform1f(arr[4], l->constant);
            glUniform1f(arr[5], l->linear);
            glUniform1f(arr[6], l->quadratic);
            memcpy(l->arr, arr, sizeof(arr));
            break;
        }
        case SPOT:
        {
            Spot_light *l = p;
            glUseProgram(l->shader_program);
            unsigned int arr[10];
            memcpy(&str[len+1], "position", sizeof "position");
            arr[0]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "direction", sizeof "direction");
            arr[1] = glGetUniformLocation(l->shader_program, str);

            memcpy(&str[len+1], "ambient", sizeof "ambient");
            arr[2] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "diffuse", sizeof "diffuse");
            arr[3] = glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "specular", sizeof "specular");
            arr[4]=glGetUniformLocation(l->shader_program, str);

            memcpy(&str[len+1], "constant", sizeof "constant");
            arr[5]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "linear", sizeof "linear");
            arr[6]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "quadratic", sizeof "quadratic");
            arr[7]=glGetUniformLocation(l->shader_program, str);

            memcpy(&str[len+1], "cutoff", sizeof "cutoff");
            arr[8]=glGetUniformLocation(l->shader_program, str);
            memcpy(&str[len+1], "outer_cutoff", sizeof "outer_cutoff");
            arr[9]=glGetUniformLocation(l->shader_program, str);

            glUniform3fv(arr[0], 1, l->position);
            glUniform3fv(arr[1], 1, l->direction);
            glUniform3fv(arr[2], 1, l->ambient);
            glUniform3fv(arr[3], 1, l->diffuse);
            glUniform3fv(arr[4], 1, l->specular);

            glUniform1f(arr[5], l->constant);
            glUniform1f(arr[6], l->linear);
            glUniform1f(arr[7], l->quadratic);

            glUniform1f(arr[8], l->cutoff);
            glUniform1f(arr[9], l->outer_cutoff);
            memcpy(l->arr, arr, sizeof(arr));
            break;
        }
        
        default:
            break;
    }
}