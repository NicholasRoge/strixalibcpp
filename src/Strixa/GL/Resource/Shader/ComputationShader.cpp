#include <Strixa/GL/Resource/Shader/ComputationShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::ComputationShader;


/* Begin Class Definition:  ComputationShader */
ComputationShader::ComputationShader(const char* filename)
: BasicShader(GL_COMPUTE_SHADER,filename)
{
}