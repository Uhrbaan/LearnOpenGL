#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "model/mesh.h"
#include "model/model.h"
#include "render/opengl.h"
#include "window/glfw.h"

void drawMesh(struct mesh m, unsigned int shader_program)
{
    for (int i=0; i<m.n_text; i++)
        bindTextureAdd(0, m.textures[i].type, m.textures[i].gl_id, shader_program);
    drawElements(shader_program, m.vao, m.n_indi);
}

void drawModel(struct model m, unsigned int shader_program)
{
    for (int i=0; i<m.n_meshes; i++)
        drawMesh(m.meshes[i], shader_program);
}

#endif