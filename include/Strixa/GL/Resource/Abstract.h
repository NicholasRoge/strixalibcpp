#ifndef Strixa_GL_Resource_Abstract_H
#define Strixa_GL_Resource_Abstract_H

#include <Strixa/GL.h>


namespace Strixa
{
    namespace GL
    {
        namespace Resource
        {
            class Abstract
            {
                /* Member Properties */
                private:
                    Context* context;
                    bool     destroyed;

                /* Member Methods */
                protected:
                    /**
                     * Frees the resource this object represents when the current OpenGL context is destroyed.  This method is called automatically and should never be called by the implementor.
                     */
                    virtual void free() = 0;

                public:
                    Abstract();

                    virtual ~Abstract();

                    void destroy();

                    bool isDestroyed() const;

                    Context* getContext() const;
            };
        }
    }
}

#endif