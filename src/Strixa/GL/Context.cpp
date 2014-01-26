#include <Strixa/GL/Context.h>

using namespace Strixa::GL;


/* Class Definition:  Context */
std::map<DWORD,Context*> Context::context_by_threadid;

Context::Context(HDC device_context)
{
    GLint result;


    /* Initialize a few things. */
    this->active_thread_id = 0;

    /* Ensure device_context is a valid device context. */
    this->device_context = device_context;
    if (this->device_context == nullptr) {
        throw std::exception("Context creation failed.  Cause:  invalid device context given.");
    }

    /* Set the pixel format. */
    int                   pixel_format_index;
    PIXELFORMATDESCRIPTOR pixel_format_descriptor = {0};


    pixel_format_descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixel_format_descriptor.nVersion = 1;
    pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
    pixel_format_descriptor.cColorBits = 24;
    pixel_format_descriptor.cAlphaBits = 8;
    pixel_format_descriptor.cDepthBits = 32;

    pixel_format_index = ChoosePixelFormat(device_context,&pixel_format_descriptor);
    if (pixel_format_index == 0) {
        throw std::exception("Context creation failed.  Cause:  Could not select a suitable pixel format.");
    }

    if (SetPixelFormat(device_context,pixel_format_index,&pixel_format_descriptor) == FALSE) {
        throw std::exception("Context creation failed.  Cause:  failed to set the device context's pixel format.");
    }

    /* Create the context itself. */
    this->rendering_context = wglCreateContext(device_context);
    if (this->rendering_context == nullptr) {
        throw std::exception("Context creation failed.  Cause:  call to wglCreateContext returned NULL.");
    }

    /* Let the context be active for the current thread. */
    if (!wglMakeCurrent(this->device_context,this->rendering_context)) {
        throw std::exception("Context creation failed.  Cause:  could not make set the current rendering context.");
    }
    this->active_thread_id = GetCurrentThreadId();

    /* Initialize GLEW */
    result = glewInit();
    if (result != GLEW_OK) {
        throw std::exception(std::string("Could not initialize GLEW.  Cause:  ").append((char*)glewGetErrorString(result)).c_str());
    }
}

Context::~Context()
{
    if (this->rendering_context != nullptr) {
        wglMakeCurrent(NULL,NULL);
        this->active_thread_id = 0;

        wglDeleteContext(this->rendering_context);
        this->rendering_context = nullptr;
    }
}

Context* Context::create(HDC device_context)
{
    Context*& context = Context::context_by_threadid[GetCurrentThreadId()];


    if (context != nullptr) {
        throw std::exception("An OpenGL context has already been created on this thread.");
    }

    context = new Context(device_context);
    return context;
}

void Context::destroy()
{
    Context*& current_context = Context::context_by_threadid[GetCurrentThreadId()];


    if (current_context == nullptr) {
        return;
    } else {
        size_t undestroyed_resource_count = current_context->resources.size();
        
        
        delete current_context;
        current_context = nullptr;
        
        if (undestroyed_resource_count > 0) {
            throw std::exception("Destroyed OpenGL context with active resources.  Please call 'destroy' on those resources to avoid memory leakage.");
        }
    }
}

Context* Context::getCurrentContext()
{
    return Context::context_by_threadid[GetCurrentThreadId()];
}


bool Context::isValid() const
{
    const DWORD calling_thread = GetCurrentThreadId();


    return calling_thread != 0 && calling_thread == this->active_thread_id;
}

void Context::validate() const
{
    if (!this->isValid()) {
        throw std::exception("Context not valid for the current thread.  Calls to GL objects must be made from the same thread they were constructed.");
    }
}

HDC Context::getDeviceContext() const
{
    return this->device_context;
}

HGLRC Context::getRenderingContext() const
{
    return this->rendering_context;
}