#ifndef Strixa_GL_Resource_Shader_ComputationShader_H
#define Strixa_GL_Resource_Shader_ComputationShader_H

#include <Strixa/GL/Resource/Shader/BasicShader.h>


namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            namespace Shader
            {
                class ComputationShader : public BasicShader
                {
                    /* Member Methods */
                    public:
                        ComputationShader(const char* source,size_t source_size = 0);
                };
            }
        }
    }
}

#endif