#include "lighting.h"
#include <glad/gl.h>

void updateMaterial(Material material)
{
    unsigned int ambiant, diffuse, specular, shininess;
    ambiant = glGetUniformLocation(material.shader_program, "material.ambiant");
    diffuse = glGetUniformLocation(material.shader_program, "material.diffuse");
    specular=glGetUniformLocation(material.shader_program, "material.specular");
    shininess=glGetUniformLocation(material.shader_program,"material.shininess");

    glUniform3fv(ambiant, 1, material.ambiant);
    glUniform3fv(diffuse, 1, material.diffuse);
    glUniform3fv(specular, 1, material.specular);
    glUniform1f(shininess, material.shininess);
}

void updateLight(Light light)
{
    unsigned int pos, ambiant, diffuse, specular;
    pos=glGetUniformLocation(light.shader_program,"light.position");
    ambiant = glGetUniformLocation(light.shader_program, "light.ambiant");
    diffuse = glGetUniformLocation(light.shader_program, "light.diffuse");
    specular=glGetUniformLocation(light.shader_program, "light.specular");

    glUniform3fv(pos, 1, light.position);
    glUniform3fv(ambiant, 1, light.ambiant);
    glUniform3fv(diffuse, 1, light.diffuse);
    glUniform3fv(specular, 1, light.specular);
}
