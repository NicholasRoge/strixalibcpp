#ifndef Strixa_GL_Resource_Shader_GeometryShader_H
#define Strixa_GL_Resource_Shader_GeometryShader_H

#include <Strixa/GL/Resource/Shader/BasicShader.h>


namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            namespace Shader
            {
                class GeometryShader : public BasicShader
                {
                    /* Member Methods */
                    public:
                        GeometryShader(const char* source,size_t source_length = 0);
                };
            }
        }
    }
}

#endif