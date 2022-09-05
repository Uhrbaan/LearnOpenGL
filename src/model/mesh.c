#include "mesh.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include "../global.h"

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
    m.n_vert = extractVertices(mesh, (float**)&m.vertices);
    m.n_indi = extractIndices (mesh, (unsigned int**)&m.indices);
    m.material = extractMaterial(mesh, scene, directory);
    generateVAO(&m);
    return m;
}