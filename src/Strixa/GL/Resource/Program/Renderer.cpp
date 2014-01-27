#include <thread>
#include <Strixa/GL/Context.h>
#include <Strixa/GL/Resource/Program/Renderer.h>
#include <Strixa/GL/Resource/Shader/FragmentShader.h>
#include <Strixa/GL/Resource/Shader/VertexShader.h>
#include <Strixa/Util/File.h>
#include <Strixa/Util/Log.h>

#include <iostream>

using Strixa::GL::Resource::Program::Renderer;
using Strixa::GL::Resource::Shader::FragmentShader;
using Strixa::GL::Resource::Shader::VertexShader;
using Strixa::Util::File;
using Strixa::Util::Log;


Renderer::Renderer()
{
    std::string shader_source;


    this->running = false;
    this->rendering = false;
    this->render_thread_id = 0;

    this->setFramerate(60);

    /* Attach the shaders. */
    shader_source = File("../default.vs.glsl").read(0);  // Hopefully I'll never have to deal with any 4g shader files...
    VertexShader vertex_shader(shader_source.c_str());

    shader_source = File("../default.fs.glsl").read(0);
    FragmentShader fragment_shader(shader_source.c_str());


    this->attachShader(vertex_shader);
    this->attachShader(fragment_shader);
    
    vertex_shader.destroy();
    fragment_shader.destroy();

    /* Link the attached shaders into the program. */
    this->linkShaders();
}

unsigned Renderer::getFramerate()
{
    return this->frame_rate;
}

bool Renderer::isRendering()
{
    return this->rendering;
}

void Renderer::main()
{
    /* Render a frame. */
    glClearColor(0.5,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES,0,3);

    SwapBuffers(this->getContext()->getDeviceContext());
}

void Renderer::render()
{
    clock_t next_frame_clock;


    this->render_thread_id = GetCurrentThreadId();

    /* Start the render loop. */
    this->running = true;
    this->rendering = true;
    while (this->running) {
        /* Determine what the clock should read when teh next frame starts. */
        next_frame_clock = clock() + this->frame_duration;

        /* Render a frame. */
        this->execute();

        /* Obtain the clock value for when the rendering finished, and sleep for the remaining frame clock ticks. */
        while (clock() < next_frame_clock) {
            std::this_thread::yield();
        }
    }
    this->rendering = false;
}

void Renderer::setFramerate(unsigned framerate)
{
    this->frame_rate = framerate;
    this->frame_duration = CLOCKS_PER_SEC / this->frame_rate;
}

void Renderer::terminate()
{
    if (this->rendering) {
        this->running = false;
        if (GetCurrentThreadId() != this->render_thread_id) {
            while (this->rendering) {
                std::this_thread::yield();
            }
        }

        this->render_thread_id = 0;
    }
}