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
                        bool   source_set;

                    /* Member Methods */
                    private:
                        virtual void free();

                    public:
                        BasicShader(GLenum type,const char* filename = nullptr);
                        
                        void compile();

                        std::string getShaderLog() const;

                        void setSource(const char* source);

                        void setSourceFile(const char* filename);

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