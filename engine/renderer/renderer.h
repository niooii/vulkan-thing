#ifndef VULKAN_STUFF_RENDERER_H
#define VULKAN_STUFF_RENDERER_H

#include "renderer_interface.h"
#include "types.h"
#include "renderer/vulkan/vk_renderer.h"

namespace Engine {

    // for proof of concept right now
    class Mesh;

    class Renderer {
    public:
        Renderer(Window& window, BackendType renderer_backend, const char* app_name);
       ~Renderer();

       Renderer(const Renderer&) = delete;
       Renderer& operator= (Renderer&) = delete;

       bool DrawFrame();

    private:
        RendererInterface* backend;
    };

}


#endif