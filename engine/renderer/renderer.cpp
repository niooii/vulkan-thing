#include "renderer.h"

namespace Engine {

    Renderer::Renderer(Window& window, BackendType renderer_backend, const char* app_name) {

        switch(renderer_backend) {

            case VULKAN:
                backend = new Vulkan::VkRenderer(app_name, window, true);
                break;

        }

    }

    Renderer::~Renderer() {
        delete backend;
    }

    bool Renderer::DrawFrame() {



    }

}