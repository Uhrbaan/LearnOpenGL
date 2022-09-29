#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../opengl.h"
#include "shading.h"

void initShading(unsigned int shader_program)
{
    initLights(shader_program);
    initMaterials(shader_program);
}