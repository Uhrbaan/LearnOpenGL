#ifndef TEXTURE_TYPES_H
#define TEXTURE_TYPES_H

#define MAX_TEXTURE_TYPES 20
enum texture_type {                                                             // based on assimps list
    none,
    diffuse,
    specular,
    ambient,
    emissive,
    normals,
    shininess,
    opacity,
    displacement,
    lightmap,
    reflection,
    base_color,
    normal_camera,
    emission_color,
    metalic,
    diffuse_roughness,
    ambient_occlusion,
    sheen,
    clear_coat,
    transmission,
    unknown
};

const char *textureType2str(int n)
{
    switch (n)
    {
        case none:              return "none";              break;
        case diffuse:           return "diffuse";           break;
        case specular:          return "specular";          break;
        case ambient:           return "ambient";           break;
        case emissive:          return "emissive";          break;
        case normals:           return "normals";           break;
        case shininess:         return "shininess";         break;
        case opacity:           return "opacity";           break;
        case displacement:      return "displacement";      break;
        case lightmap:          return "lightmap";          break;
        case reflection:        return "reflection";        break;
        case base_color:        return "base_color";        break;
        case normal_camera:     return "normal_camera";     break;
        case emission_color:    return "emission_color";    break;
        case metalic:           return "metalic";           break;
        case diffuse_roughness: return "diffuse_roughness"; break;
        case ambient_occlusion: return "ambient_occlusion"; break;
        case sheen:             return "sheen";             break;
        case clear_coat:        return "clear_coat";        break;
        case transmission:      return "transmission";      break;
        case unknown:           return "unknown";           break;
        default: break;
    }
    return NULL;
}
#endif