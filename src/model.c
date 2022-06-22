#include "utils/utils.h"
#include "model.h"
#include "gl.h"
#include <string.h>

static struct loaded_textures loaded_textures = {0};
struct texture *loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                     char *type_str)
{
    int n = aiGetMaterialTextureCount(mat, type);
    struct texture *t = calloc(n, sizeof(struct texture));
    for (int i=0; i<n; i++)
    {
        aiString str;
        aiGetMaterialTexture(mat, type, i, &str, 
                             NULL, NULL, NULL, NULL, NULL, NULL);
        // load texture if not already loaded
        bool skip = false;
        for (int j=0; j<loaded_textures.n; j++)
        {
            if (strcmp(loaded_textures.textures[j].path, str.data) == 0)
            {
                t[i].id = loaded_textures.textures[j].id;
                t[i].type = type_str;
                t[i].path = str.data;
                skip = true;
                break;
            }
            if (!skip)
            {
                t[i].path = str.data;
                t[i].type = type;
                t[i].id = file2texture(t[i].path, GL_RGB, GL_TEXTURE_2D);
                // dynamic array
                if (loaded_textures.n+1 > loaded_textures.n_tot) // -> realloc
                {
                    size_t new_size;
                    if (!loaded_textures.n_tot)
                        new_size = 1;
                    else
                        new_size = loaded_textures.n_tot*2;

                    struct texture *p = NULL;
                    p = realloc(loaded_textures.textures, 
                                sizeof(struct texture) * new_size);
                    if (!p)
                    {
                        fprintf(ERROR_MSG_LOG("failed to extend array", 
                                "new_size=%zu\n"), new_size);
                        exit(EXIT_FAILURE);
                    }
                    loaded_textures.n_tot = new_size;
                    loaded_textures.textures = p;
                }
                loaded_textures.textures[loaded_textures.n] = t[i];
                loaded_textures.n++;
            }
        }
    }
    return t;
}

Mesh processMesh(aiMesh *mesh, const aiScene *scene)
{
    Mesh m = {0};

    m.vn = mesh->mNumVertices;
    m.vertices = malloc(sizeof(struct vertex) * m.vn);
    // process vertex pos & normals & texture_coo
    for (int i=0; i<m.vn; i++)
    {
        memcpy(m.vertices[i].position, (float*)&mesh->mVertices[i], 
               sizeof(vec3));
        memcpy(m.vertices[i].normal, (float*)&mesh->mNormals[i], 
               sizeof(vec3));
        if (mesh->mTextureCoords)
            memcpy(m.vertices[i].texure_coo, (float*)&mesh->mTextureCoords[i], 
                   sizeof(vec2));
        // else its already set at 0|0

    }
    
    // process indices
    for (int i=0; i<mesh->mNumFaces; i++)
        m.in += mesh->mFaces[i].mNumIndices;
    m.indices = malloc(sizeof(unsigned int) * m.in);
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
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        struct texture *diffuse, *specular;
        diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                       "diffuse");
        specular = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                        "specular");
        // add the textures to the mesh
        // add first diffuse then specular in order in the textures array
        // haha good fun in c
    }

    return m;
}

void processNode(Model *model, aiNode *node, const aiScene *scene)
{
    int i;
    // process all nodes mesh, add them to meshes
    for (i=0; i<node->mNumChildren; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes[i] = processMesh(mesh, scene);
    }
    // process all the children
    for (i=0; i<node->mNumChildren; i++)
        processMesh(node->mChildren[i], scene);
}

void loadModel(Model *model, const char *path)
{
    /* creates the scene (root of the data struct) from a file
     * other useful flags:
     *  - aiProcess_GenNormals
     *  - aiProcess_SplitLargeMeshes
     *  - aiProcess_OptimizeMeshes
     * -> documentation  assimp.sourceforge.net/lib_html/postprocess_8h.html  */
    const aiScene *scene = aiImportFile(path, aiProcess_Triangulate | 
                                              aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        !scene->mRootNode)
    {
        fprintf(stderr, ERROR_MSG_LOG("failed loading the scene struct", 
                                      aiGetErrorString()));
        return;
    }
    else
    {
        // aiScene Root is not loading corectly
        ptrdiff_t last_slash = (strrchr(path, '/')-path);
        model->directory = calloc(last_slash+1, sizeof(char));
        memcpy(model->directory, path, sizeof(char) * last_slash);
        model->meshes = malloc(scene->mRootNode->mNumChildren * sizeof(void*));
        model->nm     = scene->mRootNode->mNumChildren;

        processNode(model, scene->mRootNode, scene);

        return;
    }
}

void drawModel(Model *model, unsigned int shader_program)
{
    for (int i=0; i<model->nm; i++)
        drawMesh(&(model->meshes[i]), shader_program);
}

void destroyModel(Model *model)
{
    // freenig all memory and set ponters to NULL
}