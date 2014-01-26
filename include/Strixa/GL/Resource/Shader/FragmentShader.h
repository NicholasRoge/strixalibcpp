#ifndef Strixa_GL_Resource_Shader_FragmentShader_H
#define Strixa_GL_Resource_Shader_FragmentShader_H

#include <Strixa/GL/Resource/Shader/BasicShader.h>


namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            namespace Shader
            {
                class FragmentShader : public BasicShader
                {
                    /* Member Methods */
                    public:
                        FragmentShader(const char* filename = nullptr);
                };
            }
        }
    }
}

#endif