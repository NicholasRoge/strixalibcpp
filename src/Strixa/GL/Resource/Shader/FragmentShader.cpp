#include <Strixa/GL/Resource/Shader/FragmentShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::FragmentShader;


/* Begin Class Definition:  FragmentShader */
FragmentShader::FragmentShader(const char* source,size_t source_length)
: BasicShader(GL_FRAGMENT_SHADER,source,source_length)
{
}