#include <Strixa/GL/Resource/Abstract.h>

#include <Strixa/GL/Context.h>

#include <iostream>

using namespace Strixa::GL;


/* Class Definition:  Abstract */
Resource::Abstract::Abstract()
{
    this->destroyed = false;

    /* Get the current thread's OpenGL context. */
    this->context = Context::getCurrentContext();
    if (this->context == nullptr) {
        throw std::exception("No OpenGL context available in current thread.");
    }

    /* Let the context know about our newly created resource. */
    this->context->resources.push_back(this);
}

Resource::Abstract::~Abstract()
{
    if (!this->destroyed) {
        std::cout << "WARNING:  Failed to call 'destroy()' on OpenGL resource before resource object was deconstructed.  Memory leakage will have occured." << std::endl;
    }
}

void Resource::Abstract::destroy()
{
    /* Method Start */
    if (!this->destroyed) {
        /* Ensure we're allowed to make GL calls from teh current context. */
        this->context->validate();

        this->free();
        this->context->resources.remove(this);

        this->context = nullptr;
        this->destroyed = true;
    }
}

Context* Resource::Abstract::getContext() const
{
    return this->context;
}

bool Resource::Abstract::isDestroyed() const
{
    return this->destroyed;
}