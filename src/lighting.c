#include "lighting.h"
#include "gl.h"

void updateMaterial(Material material)
{
    glUseProgram(material.shader_program);
    setUniform(material.shader_program, GL_FLOAT, "material.shininess", 
               material.shininess);
    setUniform(material.shader_program, GL_INT, "material.diffuse", 0);
    setUniform(material.shader_program, GL_INT, "material.specular", 1);
    setUniform(material.shader_program, GL_INT, "material.emission", 2);
}

void updateLight(Light light)
{
    glUseProgram(light.shader_program);
    unsigned int pos, ambient, diffuse, specular;
    pos=glGetUniformLocation(light.shader_program,"light.position");
    ambient = glGetUniformLocation(light.shader_program, "light.ambient");
    diffuse = glGetUniformLocation(light.shader_program, "light.diffuse");
    specular=glGetUniformLocation(light.shader_program, "light.specular");

    glUniform3fv(pos, 1, light.position);
    glUniform3fv(ambient, 1, light.ambient);
    glUniform3fv(diffuse, 1, light.diffuse);
    glUniform3fv(specular, 1, light.specular);
}