#ifndef Strixa_GL_Resource_Program_Renderer_H
#define Strixa_GL_Resource_Program_Renderer_H

#include <Strixa/GL/Resource/Program/Abstract.h>

#include <time.h>

namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            namespace Program
            {
                class Renderer : public Abstract
                {
                    /* Member Properties */
                    private:
                        volatile unsigned frame_rate; // In seconds
                        volatile clock_t  frame_duration; // In clock ticks
                        DWORD             render_thread_id;
                        volatile bool     rendering;
                        volatile bool     running;

                    /* Member Methods */
                    private:
                        virtual void main();                        

                    public:
                        Renderer();

                        unsigned getFramerate();

                        bool isRendering();

                        /**
                         * Runs a loop which calls the execute method at a specified interval.
                         */
                        void render();

                        void setFramerate(unsigned framerate);

                        void terminate();
                };
            }
        }
    }
}

#endif