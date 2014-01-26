#include <Strixa/GL/Resource/Shader/GeometryShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::GeometryShader;


/* Begin Class Definition:  GeometryShader */
GeometryShader::GeometryShader(const char* filename)
: BasicShader(GL_GEOMETRY_SHADER,filename)
{
}