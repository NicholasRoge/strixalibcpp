#include <Strixa/GL/Resource/Shader/GeometryShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::GeometryShader;


/* Begin Class Definition:  GeometryShader */
GeometryShader::GeometryShader(const char* source,size_t source_length)
: BasicShader(GL_GEOMETRY_SHADER,source,source_length)
{
}