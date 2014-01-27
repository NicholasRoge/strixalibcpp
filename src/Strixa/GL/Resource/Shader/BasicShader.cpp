#include <Strixa/GL/Resource/Shader/BasicShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::BasicShader;


/* Class Definition:  BasicShader */
BasicShader::BasicShader(GLenum type,const char* source,size_t source_length)
{
    GLint result;


    /* Initial assertions. */
    assert(source != nullptr);
    assert((int)source_length >= 0);  // Because glShaderSource normally takes a GLint (int), our the MSB on source_length cannot be used.

    /* Obtain a shader handle. */
    this->shader_id = glCreateShader(type);
    if (this->shader_id == 0) {
        throw std::exception("Failed to generate a valid shader handle.");
    }

    /* Set the shader's source to what was given. */
    glShaderSource(this->shader_id,1,&source,(int*)&source_length);
    
    /* Attempt to compile the shader. */
    glCompileShader(this->shader_id);

    /* Determine whether or not the shader compilation was successful. */
    glGetShaderiv(this->shader_id,GL_COMPILE_STATUS,&result);
    if (result != GL_TRUE) {
        std::string message;


        message.append("Shader failed to compile.  Cause:  ").append(this->getShaderLog());
        throw std::exception(message.c_str());
    }
}

void BasicShader::free()
{
    if (this->shader_id != 0) {
        glDeleteShader(this->shader_id);
        this->shader_id = 0;
    }
}

std::string BasicShader::getShaderLog() const
{
    char*       buffer;
    GLint       length;
    std::string log;


    /* Invoke the message handler if there is messages. */
    glGetShaderiv(this->shader_id,GL_INFO_LOG_LENGTH,&length);
    if (length > 0) {
        buffer = new char[length];
        glGetProgramInfoLog(this->shader_id,length,nullptr,buffer);
        log.assign(buffer,length);
        delete[] buffer;
    } else {
        log.assign("Shader log is empty.");
    }

    return log;
}

BasicShader::operator GLuint() const
{
    return this->shader_id;
}