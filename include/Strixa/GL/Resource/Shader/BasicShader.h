#ifndef Strixa_GL_Resource_Shader_BasicShader_H
#define Strixa_GL_Resource_Shader_BasicShader_H

#include <Strixa/GL/Resource/Abstract.h>

#include <string>


namespace Strixa
{
	namespace GL
	{
        namespace Resource
        {
            namespace Shader
            {
                class BasicShader : public Resource::Abstract
                {
                    /* Member Properties */
                    private:
                        GLuint shader_id;

                    /* Member Methods */
                    private:
                        virtual void free();

                    public:
                        BasicShader(BasicShader&& move);

                        BasicShader(GLenum type,const char* source,size_t source_length = 0);
                        
                        std::string getShaderLog() const;

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