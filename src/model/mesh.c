#include "mesh.h"
#include "../utils/utils.h"
#include <stdbool.h>

// dynamic array of textures
struct
{
    unsigned int n_elements;
    size_t total_sz; // total sz of dynamic array 
    struct texture *data;
} loaded_textures={0};

unsigned int loadMaterialTextures(struct texture ***textures,
                                 const struct aiMaterial *material,
                                 enum aiTextureType ai_texture_type, 
                                 const char *directory);

struct mesh generateMesh(struct aiMesh *mesh, const struct aiScene *scene, 
                         const char *directory)
{
    struct mesh m = {0};
    m.n_vert = mesh->mNumVertices;
    m.vertices = calloc(m.n_vert, sizeof(struct vertex));

    // process vertex pos & normals & texture_coo
    for (int i=0; i<m.n_vert; i++)
    {
        m.vertices[i].pos[0]  = mesh->mVertices[i].x;
        m.vertices[i].pos[1]  = mesh->mVertices[i].y;
        m.vertices[i].pos[2]  = mesh->mVertices[i].z;

        m.vertices[i].norm[0] = mesh->mNormals[i].x;
        m.vertices[i].norm[1] = mesh->mNormals[i].y;
        m.vertices[i].norm[2] = mesh->mNormals[i].z;

        m.vertices[i].uv[0]   = mesh->mTextureCoords[0][i].x;
        m.vertices[i].uv[1]   = mesh->mTextureCoords[0][i].y;
        
    }
    
    // process indices
    for (int i=0; i<mesh->mNumFaces; i++)
        m.n_indi += mesh->mFaces[i].mNumIndices;
    m.indices = malloc(sizeof(unsigned int) * m.n_indi);
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
        struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];  // TODO handle more types of textures
        int d=0, s=0;
        struct texture **diffuse=NULL, **specular=NULL;
        d = loadMaterialTextures(&diffuse, material, aiTextureType_DIFFUSE, 
                                 directory);
        m.n_text++;
        s = loadMaterialTextures(&specular, material, aiTextureType_SPECULAR, 
                                 directory);
        m.n_text++;
        size_t texturesz = sizeof(struct texture) * d * s;
        m.textures = malloc(texturesz);
        memcpy(m.textures, diffuse, sizeof(struct texture)*d);
        memcpy(&m.textures[d], specular, sizeof(struct texture)*s);
    }
    // once done, generate vao from global
    #include "../global.h"
    generateVAO(&m);
    return m;
}

#include "../render/opengl.h"

// TODO rewrite this hidious function & implement material instead of textures
unsigned int loadMaterialTextures(struct texture ***textures,
                                  const struct aiMaterial *material,
                                  enum aiTextureType ai_texture_type,
                                  const char *directory)
{
    int n = aiGetMaterialTextureCount(material, ai_texture_type);
    *textures = calloc(n, sizeof(struct texture*));
    assert(textures);

    for (int i=0; i<n; i++)
    {
        struct aiString str = {0};
        struct texture t    = {0};
        float shininess = 1.f;
        bool skip=false; unsigned int empty[2]={0};
        aiGetMaterialTexture(material, ai_texture_type, i, &str,                // get the name of the texture
                             NULL, NULL, NULL, NULL, NULL, NULL);
        strncpy(t.path, directory, 50); // src/res/backpack                     // get full path of texture
        strcat(t.path, "/");            // src/res/backpack/
        strncat(t.path, str.data, 48);  // src/res/backpack/diffuse.jpg
        for (int j=0; j<loaded_textures.n_elements; j++)
        {
            if (!strncmp(loaded_textures.data[j].path, t.path, 99))
            {
                *textures[i] = &loaded_textures.data[j];
                skip = true;
                break;
            }
            if (loaded_textures.data[j].gl_id==0)
            {
                empty[0] = 1;
                empty[1] = j;
            }
        }
        if (!skip)
        {
            if (!empty[0])                                                      // if texture is not present, load it 
            {                                                                   // into the dynamic array
                t.type = ai_texture_type;
                t.gl_id = loadTexture(t.path);
                loaded_textures.total_sz = 
                da_push((void*)&loaded_textures.data, 
                    (size_t)sizeof(struct texture)*loaded_textures.n_elements,
                    loaded_textures.total_sz,
                    &t, sizeof(struct texture));
                *textures[i] =
                    &loaded_textures.data[loaded_textures.n_elements++];
            }
            else 
            {
                strncpy(loaded_textures.data[empty[1]].path, str.data, 99); 
                loaded_textures.data[empty[1]].type = ai_texture_type;
                loaded_textures.data[empty[1]].gl_id = 
                    loadTexture(loaded_textures.data[empty[1]].path);
                *textures[i] = &loaded_textures.data[empty[1]];
            }
            if (t.gl_id==0 || t.type==0)
                fprintf(stderr, 
                    TERM_COL_ERROR("error: ")
                    TERM_COL_INFO("texture wasn't proprely generated: ")
                    "texture id: %d, texture type: %d",
                    t.gl_id, t.type);
        }
    }
    return n;
}