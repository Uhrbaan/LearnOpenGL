#include "mesh.h"
#include "../utils/utils.h"
#include <stdbool.h>

// dynamic array of textures
struct
{
    unsigned int n_elements;
    size_t total_sz; // total sz of dynamic array 
    unsigned int (*txt_id_fn)(const char*); // fn pointer to loadTexture, its a little stupid but idk...
    struct texture *data;
} loaded_textures={0};

void setGlTextureId_fn(unsigned int (*fn)(const char*))
{
    loaded_textures.txt_id_fn = fn;
}

unsigned int loadMaterialTextures(struct texture *textures,
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
        struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        int d = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE), 
            s = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
        m.n_text = d+s;
        m.textures = calloc(sizeof(struct texture), m.n_text);
        loadMaterialTextures(m.textures, material, 
                             aiTextureType_DIFFUSE, directory);
        loadMaterialTextures(&m.textures[d], material, 
                             aiTextureType_SPECULAR, directory);
    }
    return m;
}

unsigned int loadMaterialTextures(struct texture *textures,
                                  const struct aiMaterial *material,
                                  enum aiTextureType ai_texture_type,
                                  const char *directory)
{
    int n = aiGetMaterialTextureCount(material, ai_texture_type);
    for (int i=0; i<n; i++)
    {
        struct aiString str;
        bool skip=false; unsigned int empty[2]={0};
        aiGetMaterialTexture(material, ai_texture_type, i, &str, 
                             NULL, NULL, NULL, NULL, NULL, NULL);
        char tmp[200]={0};
        strncpy(tmp, directory, 199);
        tmp[strlen(tmp)] = '/';
        // textures[i].gl_id -> need to create glTexture in global
        for (int j=0; j<loaded_textures.n_elements; j++)
        {
            if (!strncmp(loaded_textures.data[j].path, str.data, 99))
            {
                textures[i] = loaded_textures.data[j];
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
            if (!empty[0])
            {
                // FIXME on first pass the first diffuse shader texture is corrupted, the one in loaded shaders isn't
                struct texture t;
                strncpy(t.path, str.data, 99); 
                strncat(tmp, t.path, 199);
                t.type = ai_texture_type;  
                t.gl_id = loaded_textures.txt_id_fn(tmp);
                loaded_textures.total_sz = 
                da_push((void*)&loaded_textures.data, 
                        (size_t)sizeof(struct texture)*loaded_textures.n_elements,
                        loaded_textures.total_sz,
                        &t, sizeof(struct texture));
                textures[i] = loaded_textures.data[loaded_textures.n_elements];
                loaded_textures.n_elements++;
            }
            else 
            {
                strncpy(loaded_textures.data[empty[1]].path, str.data, 99);
                strncat(tmp, loaded_textures.data[empty[1]].path, 199);
                loaded_textures.data[empty[1]].type = ai_texture_type;
                loaded_textures.data[empty[1]].gl_id =
                    loaded_textures.txt_id_fn(tmp);
                textures[i] = loaded_textures.data[empty[1]];
            }
        }
    }
    return n;
}