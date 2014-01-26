#ifndef Strixa_GL_Resource_Program_Abstract_H
#define Strixa_GL_Resource_Program_Abstract_H

#include <initializer_list>
#include <map>
#include <string>
#include <Strixa/GL.h>
#include <Strixa/GL/Context.h>
#include <Strixa/GL/Resource/Abstract.h>
#include <strixa/GL/Resource/Buffer.h>
#include <Strixa/GL/Resource/Shader/BasicShader.h>


namespace Strixa
{
	namespace GL
	{
        namespace Attribute
        {
            class BasicAttribute
            {
                /* Member Properties */
                protected:
                    GLsizei     element_count;
                    GLsizei     element_size;
                    const char* name;
                    GLboolean   normalize;
                    GLenum      type;

                /* Member Methods */
                public:
                    BasicAttribute(const char* name,GLsizei element_size,GLenum type,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);

                    GLsizei getElementCount() const;

                    GLsizei getElementSize() const;

                    const char* getName() const;

                    GLsizei getSize() const;

                    GLenum getType() const;

                    GLboolean isNormalized() const;
            };

            class ByteAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    ByteAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class DoubleAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    DoubleAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class FloatAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    FloatAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class IntAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    IntAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class ShortAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    ShortAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class UByteAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    UByteAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class UIntAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    UIntAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };

            class UShortAttribute : public BasicAttribute
            {
                /* Member Methods */
                public:
                    UShortAttribute(const char* name,GLsizei element_count = 1,GLboolean normalize = GL_FALSE);
            };
        }

        namespace Resource
        {
            namespace Program
            {
                class Abstract : public Resource::Abstract
		        {
                    /* Member Properties */
			        private:
                        GLuint program_id;
                        GLuint array_id;

			        /* Member Methods */
                    private:
                        virtual void free();

                        /**
                        * Called when the users requests that this program be executed.
                        */
                        virtual void main() = 0;

			        public:
                        Abstract();

                        void attachShader(const Shader::BasicShader& shader);

                        void bindAttributes(Buffer& buffer,std::initializer_list<Attribute::BasicAttribute> attributes);

                        void execute();

                        std::string getProgramLog();
                
                        void linkShaders();

                        void setUniformValue(const char* name,GLfloat v0);
                        
                        /**
                        * Returns the OpenGL handle to this object.
                        *
                        * @return The OpenGL handle to this object.
                        */
                        operator GLuint() const;
		        };
            }
        }
	}
}

#endif