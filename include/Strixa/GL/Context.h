#ifndef Strixa_GL_Context_H
#define Strixa_GL_Context_H

#include <map>
#include <list>
#include <Windows.h>
#include <GL/glew.h>  //Must be included after Windows.h

#include <Strixa/GL/Resource/Abstract.h>
#include <Strixa/Util/Log.h>


namespace Strixa
{
    namespace GL
    {
        class Context
        {
            /* Friend Declarations */
            friend class Resource::Abstract;

            /* Static Properties */
            private:
                static std::map<DWORD,Context*> context_by_threadid;
                //static thread_local Context context;  // Note:  Under the visual studio 2013 compiler, thread_local must be defined as a macro having value __declspec(thread)

            /* Static Methods */
            public:
                /**
                 * Creates an OpenGL context on the current thread.
                 *
                 * @param device_context TODO
                 *
                 * @return Returns a handle to the current thread's device context.
                 */
                static Context* create(HDC device_context);
                
                /**
                * Destroy's the current thread's OpenGL context.
                */
                static void destroy();

                /**
                 * Gets the current thread's device context.
                 */
                static Context* getCurrentContext();

            /* Member Properties */
            private:
                DWORD                          active_thread_id;
                HDC                            device_context;
                HGLRC                          rendering_context;
                std::list<Resource::Abstract*> resources;

            /* Member Methods */
            private:
                Context(HDC device_context);

                ~Context();

            public:
                HDC getDeviceContext() const;

                HGLRC getRenderingContext() const;

                bool isValid() const;

                void validate() const;
        };
    }
}

#endif