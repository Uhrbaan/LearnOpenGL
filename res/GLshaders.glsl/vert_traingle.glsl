#version 460 core // version of the shader
// declare all input vertex attributes with 'in' as a 3d vector with name aPos 
layout (location = 0) in vec3 aPos;

void main()
{
    // GLSL supports max sized vectore of vec4 -> component x, y, z, w
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}