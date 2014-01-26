#include <Strixa/GL/Resource/Shader/BasicShader.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using Strixa::GL::Resource::Shader::BasicShader;


/* Begin Class Definition:  BasicShader */
BasicShader::BasicShader(GLenum type,const char* filename)
{
    /*  */
    this->shader_id = glCreateShader(type);
    if (this->shader_id == 0) {
        throw std::exception("Failed to generate a valid shader handle.");
    }

    /* If a filename was given, set the source of this shader to that files contents. */
    if (filename != nullptr) {
        this->setSourceFile(filename);
    }
}

void BasicShader::compile()
{
    GLint result;


    /* Ensure the shader's source has been set. */
    if (!this->source_set) {
        throw std::exception("Shader source has not yet been set.  Please do so using 'Shader#setSource(const char*)' or 'Shader#setSourceFile(const char*)' before attempting to compile.");
    }

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

void BasicShader::setSource(const char* source)
{
    assert(source != nullptr);

    glShaderSource(this->shader_id,1,&source,NULL);
    this->source_set = true;
}

void BasicShader::setSourceFile(const char* filename)
{
    assert(filename != nullptr);
    //assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);


    FILE*  file;
    char*  source;
    size_t source_size;


    /* Open our file. */
    fopen_s(&file,filename,"rb");
    if (file == nullptr) {
        throw std::exception("Failed to open requested shader source file.");
    }

    /* Determine the size of the file we're trying to read. */
    fseek(file,0,SEEK_END);  // TODO:  Non-portable?
    source_size = ftell(file);
    fseek(file,0,SEEK_SET);

    /* Retrieve its contents. */
    source = new char[source_size + 1];
    source[source_size] = '\0';
    fread(source,1,source_size,file);
    fclose(file);

    /* Give the user their shader. */
    this->setSource(source);

    /* Get rid of the temporary buffer. */
    delete[] source;
}

BasicShader::operator GLuint() const
{
    return this->shader_id;
}