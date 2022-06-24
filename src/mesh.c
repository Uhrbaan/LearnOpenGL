#include "mesh.h"

void initMesh(Mesh *mesh);

Mesh createMesh(struct vertex *vertices, int vn, 
                unsigned int *indices, int in,
                struct texture *textures, int tn)
{
    Mesh mesh = {0};
    mesh.vertices = vertices; mesh.n_vert = vn;
    mesh.indices = indices;   mesh.n_ind = in;
    mesh.textures = textures; mesh.n_tex = tn;
    initMesh(&mesh);
    return mesh;    
}

#include <glad/gl.h>
void initMesh(Mesh *mesh)
{
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->n_vert * sizeof (struct vertex),
                 mesh->vertices, GL_STATIC_DRAW);

    glBindVertexArray(ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_ind * sizeof (unsigned int),
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

    // "return" values
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
}

/* named -> texture_diffuse_N/specular_N */
#include <string.h>
void drawMesh(Mesh *mesh, unsigned int shader_program)

{
    int diffuse_n = 1, specular_n = 1, n=0, s_loc=0;
    char name[100];
    for (int i=0; i<mesh->n_tex; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        if (!strcmp(mesh->textures[i].type, "diffuse"))
            n = diffuse_n++;
        else if (!strcmp(mesh->textures[i].type, "specular"))
            n = specular_n++;

        snprintf(name, 99, "material.texture_%s_%d", mesh->textures[i].type, n);
        glUseProgram(shader_program);
        s_loc = glGetUniformLocation(shader_program, name);
        glUniform1ui(s_loc, mesh->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->n_tex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}