#ifndef Strixa_GL_H
#define Strixa_GL_H

#include <Windows.h>
#include <GL/glew.h>

namespace Strixa
{
    namespace GL
    {
        class Context;

        namespace Resource
        {
            class Abstract;
            class Buffer;

            namespace Shader
            {
                class BasicShader;
                class ComputationShader;
                class FragmentShader;
                class GeometryShader;
                class VertexShader;
            }

            namespace Program
            {
                class Abstract;
                class Renderer;
            }            
        }
    }
}

#endif