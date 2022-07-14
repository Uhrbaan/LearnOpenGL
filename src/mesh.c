#include "mesh.h"
#include "utils/utils.h"

// crée un objet Mesh 
Mesh generateMesh(struct aiMesh *mesh, const struct aiScene *scene, const char *directory)
{
    Mesh m = {0};
    m.n_vert = mesh->mNumVertices;
    m.vertices = calloc(m.n_vert, sizeof(struct vertex));

    // process vertex pos & normals & texture_coo
    for (int i=0; i<m.n_vert; i++)
    {
        m.vertices[i].position[0]   = mesh->mVertices[i].x;
        m.vertices[i].position[1]   = mesh->mVertices[i].y;
        m.vertices[i].position[2]   = mesh->mVertices[i].z;

        m.vertices[i].normal[0]     = mesh->mNormals[i].x;
        m.vertices[i].normal[1]     = mesh->mNormals[i].y;
        m.vertices[i].normal[2]     = mesh->mNormals[i].z;

        m.vertices[i].texure_coo[0] = mesh->mTextureCoords[0][i].x;
        m.vertices[i].texure_coo[1] = mesh->mTextureCoords[0][i].y;
    }
    
    // process indices
    for (int i=0; i<mesh->mNumFaces; i++)
        m.n_ind += mesh->mFaces[i].mNumIndices;
    m.indices = malloc(sizeof(unsigned int) * m.n_ind);
    void *p = &m.indices[0];
    for (int i=0; i<mesh->mNumFaces; i++)
    {
        size_t s = sizeof(unsigned int)*mesh->mFaces[i].mNumIndices;
        memcpy(p, mesh->mFaces[i].mIndices, s);
        p += s; // move pointer to end of copy adress
    }

    // material
    if (mesh->mMaterialIndex >= 0)
    {
        struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        int d, s;
        struct texture *diffuse, *specular;
        d = loadMaterialTextures(&diffuse, material, aiTextureType_DIFFUSE,
                                       "diffuse", directory);
        s = loadMaterialTextures(&specular, material, aiTextureType_SPECULAR,
                                        "specular", directory);

        m.n_tex = d+s;
        m.textures = malloc(sizeof(struct texture)*m.n_tex);
        void *p = &m.textures[0];
        for (int i=0; i<d; i++)
        {
            memcpy(p, &diffuse[i], sizeof(struct texture));
            p+=sizeof(struct texture);
        }
        for (int i=0; i<s; i++)
        {
            memcpy(p, &specular[i], sizeof(struct texture));
            p+=sizeof(struct texture);
        }
    }

    // generate vao, vbo, ebo from mesh data
    initMesh(&m);

    return m;
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
// TODO optimize to remove strcmp & string functions & getUniLoc
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

        snprintf(name, 99, "texture_%s_%d", mesh->textures[i].type, n);
        glUseProgram(shader_program);
        s_loc = glGetUniformLocation(shader_program, name);
        glUniform1ui(s_loc, i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }
    // draw mesh
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->n_ind, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}