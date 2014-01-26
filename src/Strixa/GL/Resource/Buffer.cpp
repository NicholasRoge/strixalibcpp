#include <Strixa/GL/Resource/Buffer.h>

#include <Strixa/GL/Context.h>

#include <assert.h>
#include <iostream>

using Strixa::GL::Resource::Buffer;


Buffer::Buffer(size_t length)
{
    /* Obtain a handle to the GL buffer. */
    glGenBuffers(1,&this->buffer_id);
    if (this->buffer_id == 0) {
        throw std::exception("Buffer creation failed.  Cause:  Could not generate server-side buffer.");
    }

    /* Allocate the "client-side" buffer. */
    this->buffer = new char[length]();
    this->buffer_length = length;

    /* Allocate the "server-side" buffer. */    
    glBindBuffer(GL_ARRAY_BUFFER,this->buffer_id);
    glBufferData(GL_ARRAY_BUFFER,this->buffer_length,this->buffer,GL_DYNAMIC_DRAW);  // TODO:  Allow for modification of the final parameter of this call.
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Buffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER,this->buffer_id);
}

void Buffer::free()
{
    if (this->buffer_id != 0) {
        glDeleteBuffers(1,&this->buffer_id);
        
        delete[] this->buffer;
        this->buffer = nullptr;
        this->buffer_length = 0;
    }
}

void Buffer::invalidate()
{
    this->invalidate(0,this->buffer_length);
}

void Buffer::invalidate(GLsizei start_offset,GLsizei length)
{
    assert(start_offset >= 0 && start_offset + length <= this->buffer_length);
}

void Buffer::release()
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Buffer::set(GLsizei start_offset,const void* data,GLsizei data_size)
{
    this->invalidate(start_offset,data_size);
    memcpy(this->buffer + start_offset,data,data_size);
}

void Buffer::update()
{
    /* Ensure we're allowed to make GL calls from teh current context. */
    this->getContext()->validate();

    /* Method Start */
    /*  */
    // TODO:  For now, we're just updating the whole buffer at a time.  In the future, however, it is absolutely essential that we only update teh sections of the array that have changed.
    glBindBuffer(GL_ARRAY_BUFFER,this->buffer_id);
    glBufferSubData(GL_ARRAY_BUFFER,0,this->buffer_length,this->buffer + 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void* Buffer::operator [](size_t offset) const
{
    assert(offset >= 0 && offset < this->buffer_length);

    return this->buffer + offset;
}

Buffer::operator GLuint() const
{
    return this->buffer_id;
}

Buffer::operator void*() const
{
    return this->buffer;
}