#include <Strixa/GL/Resource/Shader/VertexShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::VertexShader;


/* Begin Class Definition:  VertexShader */
VertexShader::VertexShader(const char* filename)
: BasicShader(GL_VERTEX_SHADER,filename)
{
}