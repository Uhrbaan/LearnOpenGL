#include "mesh.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include "../global.h"

void generateVAO(struct mesh *mesh);

// dynamic array of textures
struct
{
    unsigned int n_elements;
    size_t total_sz; // total sz of dynamic array 
    struct texture *data;
} loaded_textures={0};

#include "../assimp_loading/assimp.h"
struct mesh generateMesh(struct aiMesh *mesh, const struct aiScene *scene, 
                         const char *directory)
{
    struct mesh m = {0};
    m.n_vert   = extractVertices (mesh, (float**)&m.vertices);
    m.n_indi   = extractIndices  (mesh, (unsigned int**)&m.indices);
    m.material = extractMaterial (mesh, scene, directory);
    m.vao      = generateVao     ((float*)m.vertices,       m.n_vert, 
                                  (unsigned int*)m.indices, m.n_indi, 
                                  &m.vbo, &m.ebo);
    return m;
}

void drawMesh(struct mesh m, unsigned int shader_program)
{
    for (int i=0; i<SUPPORTED_TEXTURE_TYPES_N; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glUseProgram(shader_program);
        if (m.material->texture_id[i])
            glUniform1i(m.material->texture_uniform[i], i); // link to n-th sampler
        glBindTexture(GL_TEXTURE_2D, m.material->texture_id[i]);
    }
    drawElements(shader_program, m.vao, m.n_indi);
}