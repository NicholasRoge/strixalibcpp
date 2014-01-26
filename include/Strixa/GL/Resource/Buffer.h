#ifndef Strixa_GL_Resource_Buffer_H
#define Strixa_GL_Resource_Buffer_H

#include <Strixa/GL.h>
#include <Strixa/GL/Resource/Abstract.h>

namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            class Buffer : public Abstract
            {
                /* Member Properties */
                private:
                    char*    buffer;         // Note:  Implemented as char* to avoid void* arithmetic
                    GLuint   buffer_id;
                    GLsizei  buffer_length;  // Size of the buffer in bytes

                /* Member Methods */
                protected:
                    virtual void free();

                public:
                    /**
                     * @param length Size in bytes of the buffer.
                     */
                    explicit Buffer(size_t length);

                    void bind();

                    void invalidate();

                    void invalidate(GLsizei start_offset,GLsizei length);

                    void release();

                    void set(GLsizei start_offset,const void* data,GLsizei data_size);

                    void update();

                    /**
                     * Accesses and returns a pointer to the byte at the given offset.
                     *
                     * @param offset Offset of the byte in the buffer you would like to access.
                     *
                     * @return Returns a pointer to the byte at the given offset.
                     */
                    void* operator [](size_t offset) const;

                    /**
                    * Returns the OpenGL handle to this object.
                    *
                    * @return The OpenGL handle to this object.
                    */
                    operator GLuint() const;

                    operator void*() const;
            };
        }
    }
}

#endif