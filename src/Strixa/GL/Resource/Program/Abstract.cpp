#include <Strixa/GL/Resource/Program/Abstract.h>

#include <Strixa/GL/Context.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>

using namespace Strixa::GL::Attribute;
using Strixa::GL::Resource::Buffer;
using Strixa::GL::Resource::Program::Abstract;
using Strixa::GL::Resource::Shader::BasicShader;


/* Class Definition:  Resource::Program */
Abstract::Abstract()
{
    /* Create a new GL program. */
    this->program_id = glCreateProgram();
    if (this->program_id == 0) {
        throw std::exception("Program creation failed.  Cause:  Failed to create an empty GL program.");
    }

    glGenVertexArrays(1,&this->array_id);
}

void Abstract::free()
{
    if (this->program_id != 0) {
        glDeleteProgram(this->program_id);
        this->program_id = 0;
    }
}

std::string Abstract::getProgramLog()
{
    char*       buffer;
    GLint       length;
    std::string log;
    

    /* Invoke the message handler if there is messages. */
    glGetProgramiv(this->program_id,GL_INFO_LOG_LENGTH,&length);
    if (length > 0) {
        buffer = new char[length];
        glGetProgramInfoLog(this->program_id,length,nullptr,buffer);
        log.assign(buffer,length);
        delete[] buffer;
    } else {
        log.assign("Program log is empty.");
    }

    return log;
}

void Abstract::bindAttributes(Buffer& buffer,std::initializer_list<BasicAttribute> attributes)
{
    /* Ensure we're allowed to make GL calls from teh current context. */
    this->getContext()->validate();

    /* Method Start */
    GLsizei full_stride;
    GLint   location;
    GLsizei offset;


    /* Determine how many bytes are in a full stride. */
    full_stride = 0;
    for (auto attribute : attributes) {
        if (attribute.getElementCount() < 1 || attribute.getElementCount() > 4) {
            throw std::exception("Binding attribute to buffer failed.  Cause:  Attribute size may contain no less than '1' element, and no greater than '4' elements.");
        }

        full_stride += attribute.getSize();
    }

    /* Bind the attributes to the buffer. */
    offset = 0;

    buffer.bind();
    glBindVertexArray(this->array_id);
    for (auto attribute : attributes) {
        location = glGetAttribLocation(*this,attribute.getName());
        if (location == -1) {
            std::string message;


            message
                .append("Binding attribute to buffer failed.  Cause:  No such attribute with name '")
                .append(attribute.getName())
                .append("' could be found.");
            throw std::exception(message.c_str());
        }

        glEnableVertexAttribArray(location);
        glVertexAttribPointer(
            location,
            attribute.getElementCount(),
            attribute.getType(),
            attribute.isNormalized(),
            full_stride - attribute.getSize(),
            (GLvoid*)offset
            );

        offset += attribute.getSize();
    }
    glBindVertexArray(0);
    buffer.release();
}

void Abstract::execute()
{
    /* Ensure we're allowed to make GL calls from teh current context. */
    this->getContext()->validate();

    /* Method Start */
    glUseProgram(this->program_id);
    glBindVertexArray(this->array_id);
    
    this->main();
    
    glBindVertexArray(0);
	glUseProgram(0);
}

void Abstract::attachShader(const BasicShader& shader)
{
    /* Ensure we're allowed to make GL calls from teh current context. */
    this->getContext()->validate();

    /* Method Start */
    glAttachShader(*this,shader);
}

void Abstract::linkShaders()
{
    /* Ensure we're allowed to make GL calls from teh current context. */
    this->getContext()->validate();

    /* Method Start */
    GLint result;


    /* Attempt to attach and link the shaders into this program. */
    glLinkProgram(this->program_id);

    /* Check to see if there were any errors. */
    glGetProgramiv(this->program_id,GL_LINK_STATUS,&result);
    if (result != GL_TRUE) {
        std::string message;


        message.append("Failed to link shaders into program.  Cause:  ").append(this->getProgramLog());
        throw std::exception(message.c_str());
    }
}

Abstract::operator GLuint() const
{
    return this->program_id;
}

/* Class Definition:  AbstractAttribute */
BasicAttribute::BasicAttribute(const char* name,GLsizei element_size,GLenum type,GLsizei element_count,GLboolean normalize)
{
    assert(name != nullptr);

    this->element_count = element_count;
    this->element_size = element_size;
    this->name = name;
    this->normalize = normalize;
    this->type = type;
}

GLsizei BasicAttribute::getElementCount() const
{
    return this->element_count;
}

GLsizei BasicAttribute::getElementSize() const
{
    return this->element_size;
}

const char* BasicAttribute::getName() const
{
    return this->name;
}

GLsizei BasicAttribute::getSize() const
{
    return this->element_count * this->element_size;
}

GLenum BasicAttribute::getType() const
{
    return this->type;
}

GLboolean BasicAttribute::isNormalized() const
{
    return this->normalize;
}

/* Class Definition:  ByteAttribute */
ByteAttribute::ByteAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLbyte),GL_BYTE,element_count,normalize)
{
}

/* Class Definition:  DoubleAttribute */
DoubleAttribute::DoubleAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLdouble),GL_DOUBLE,element_count,normalize)
{
}

/* Class Definition:  FloatAttribute */
FloatAttribute::FloatAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLfloat),GL_FLOAT,element_count,normalize)
{
}

/* Class Definition:  IntAttribute */
IntAttribute::IntAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLint),GL_INT,element_count,normalize)
{
}

/* Class Definition:  ShortAttribute */
ShortAttribute::ShortAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLshort),GL_SHORT,element_count,normalize)
{
}

/* Class Definition:  *Attribute */
UByteAttribute::UByteAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLubyte),GL_UNSIGNED_BYTE,element_count,normalize)
{
}

/* Class Definition:  UIntAttribute */
UIntAttribute::UIntAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLuint),GL_UNSIGNED_INT,element_count,normalize)
{
}

/* Class Definition:  UShortAttribute */
UShortAttribute::UShortAttribute(const char* name,GLsizei element_count,GLboolean normalize)
: BasicAttribute(name,sizeof(GLushort),GL_UNSIGNED_SHORT,element_count,normalize)
{
}



//void Abstract::setUniform(std::string name,GLfloat v1)
//{
//    /* Ensure we're allowed to make GL calls from teh current context. */
//    this->getContext()->validate();
//
//    /* Method Start */
//    GLint   location;
//
//
//    glUseProgram(this->program_id);
//    location = glGetUniformLocation(this->program_id,name.c_str());
//    if (location == -1) {
//        throw std::exception(std::string("No such uniform:  ").append(name).c_str());
//    } else {
//        glUniform1f(location,v1);
//    }
//    glUseProgram(0);
//}
//
//void Abstract::setUniform(std::string name,GLfloat v1,GLfloat v2)
//{
//    /* Ensure we're allowed to make GL calls from teh current context. */
//    this->getContext()->validate();
//
//    /* Method Start */
//    GLint   location;
//
//
//    glUseProgram(this->program_id);
//    location = glGetUniformLocation(this->program_id,name.c_str());
//    if (location == -1) {
//        throw std::exception(std::string("No such uniform:  ").append(name).c_str());
//    } else {
//        glUniform2f(location,v1,v2);
//    }
//    glUseProgram(0);
//}
//
//void Abstract::setUniform(std::string name,GLfloat v1,GLfloat v2,GLfloat v3)
//{
//    /* Ensure we're allowed to make GL calls from teh current context. */
//    this->getContext()->validate();
//
//    /* Method Start */
//    GLint   location;
//
//
//    glUseProgram(this->program_id);
//    location = glGetUniformLocation(this->program_id,name.c_str());
//    if (location == -1) {
//        throw std::exception(std::string("No such uniform:  ").append(name).c_str());
//    } else {
//        glUniform3f(location,v1,v2,v3);
//    }
//    glUseProgram(0);
//}
//
//void Abstract::setUniform(std::string name,GLfloat v1,GLfloat v2,GLfloat v3,GLfloat v4)
//{
//    /* Ensure we're allowed to make GL calls from teh current context. */
//    this->getContext()->validate();
//
//    /* Method Start */
//    GLint   location;
//
//
//    glUseProgram(this->program_id);
//    location = glGetUniformLocation(this->program_id,name.c_str());
//    if (location == -1) {
//        throw std::exception(std::string("No such uniform:  ").append(name).c_str());
//    } else {
//        glUniform4f(location,v1,v2,v3,v4);
//    }
//    glUseProgram(0);
//}