#include <Strixa/GL/Resource/Shader/FragmentShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::FragmentShader;


/* Begin Class Definition:  FragmentShader */
FragmentShader::FragmentShader(const char* filename)
: BasicShader(GL_FRAGMENT_SHADER,filename)
{
}