#version 460 core
layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos; // to calculate the fragment position in world space

void main()
{
    gl_Position = projection * view  * model * vec4(i_pos, 1.0);
    // making normal resist non-uniform scaling
    // NOTE: inverse() is expensive -> preferably to do in CPU only when needed
    normal = mat3(transpose(inverse(model))) * i_normal;
    frag_pos = vec3(model*vec4(i_pos, 1.0));
}