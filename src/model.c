#include "utils/utils.h"
#include "model.h"
#include "gl.h"
#include <string.h>

struct loaded_textures
{
    size_t n;   // number of elements
    size_t tot_sz; // total size of the dynamic array
    struct texture *textures; // data
}; static struct loaded_textures loaded_textures = {0};

// charge une texture dans l'array globale des textures
int loadMaterialTextures(struct texture **p, 
                                     aiMaterial *mat, aiTextureType type,
                                     char *type_str, const char *directory)
{
    int n = aiGetMaterialTextureCount(mat, type);
    struct texture *t = calloc(n, sizeof(struct texture));
    char path[2096];
    for (int i=0; i<n; i++)
    {
        aiString str;
        aiGetMaterialTexture(mat, type, i, &str, 
                             NULL, NULL, NULL, NULL, NULL, NULL);
        // TODO optimize
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, str.data);
        // load texture if not already loaded
        bool skip = false;
        for (int j=0; j<loaded_textures.n; j++)
        {
            if (strcmp(loaded_textures.textures[j].path, path) == 0)
            {
                t[i].id = loaded_textures.textures[j].id;
                t[i].type = type_str;
                t[i].path = path;
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            t[i].path = path;
            t[i].type = type_str;
            t[i].id = file2texture(t[i].path, GL_RGB, GL_TEXTURE_2D);
            loaded_textures.tot_sz = 
                da_push((void**)&loaded_textures.textures, 
                        loaded_textures.n++ * sizeof(struct texture), 
                        loaded_textures.tot_sz, 
                        &t[i],
                        sizeof(struct texture));
        }
    }
    *p = t;
    return n;
}

// crée un objet Mesh 
Mesh processMesh(aiMesh *mesh, const aiScene *scene, const char *directory)
{
    Mesh m = {0};
    m.n_vert = mesh->mNumVertices;
    m.vertices = malloc(sizeof(struct vertex) * m.n_vert);

    // process vertex pos & normals & texture_coo
    for (int i=0; i<m.n_vert; i++)
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
        aiMaterial  *material = scene->mMaterials[mesh->mMaterialIndex];
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

#include <libgen.h> // to get the basename / directory of path
int loadModel(Model *model, const char *path)
// checkout https://gist.github.com/tilkinsc/661d9da4a4121101a182b8880857f6fb
// pure C loading w/ assimp
{
    /* creates the scene (root of the data struct) from a file
     * other useful flags:
     *  - aiProcess_GenNormals
     *  - aiProcess_SplitLargeMeshes
     *  - aiProcess_OptimizeMeshes
     * -> documentation  assimp.sourceforge.net/lib_html/postprocess_8h.html  */
    unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs;
    const aiScene *scene = aiImportFile(path, flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mNumMeshes <= 0)
    {
        aiReleaseImport(scene);
        fprintf(stderr, ERROR_MSG_LOG("assimp could not load file", 
                                       aiGetErrorString()));
        return 1;
    }
    char directory[1048];
    strcpy(directory, path);
    dirname(directory);

    model->directory = malloc(sizeof(char)*strlen(directory));
    strcpy(model->directory, directory);

    model->n_mesh = scene->mNumMeshes;
    model->meshes = malloc(sizeof(Mesh)*model->n_mesh);

    // reading the meshes
    for (int i=0; i<model->n_mesh; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        model->meshes[i] = processMesh(mesh, scene, directory);
    }
    return 1;
}

void drawModel(Model *model, unsigned int shader_program)
{
    for (int i=0; i<model->n_mesh; i++)
        drawMesh(&model->meshes[i], shader_program);
}