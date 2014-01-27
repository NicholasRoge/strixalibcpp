#ifndef Strixa_GL_Resource_Shader_VertexShader_H
#define Strixa_GL_Resource_Shader_VertexShader_H

#include <Strixa/GL/Resource/Shader/BasicShader.h>


namespace Strixa
{
	namespace GL
	{
        namespace Resource
        {
            namespace Shader
            {
                class VertexShader : public BasicShader
                {
                    /* Member Methods */
                    public:
                        VertexShader(const char* source,unsigned source_length = 0);
                };
            }
        }
	}
}

#endif