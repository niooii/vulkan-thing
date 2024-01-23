#ifndef VULKAN_STUFF_RENDERER_H
#define VULKAN_STUFF_RENDERER_H

#include <vulkan/vulkan.h>
#include "Instance.h"

namespace VulkanRenderer {

    class Renderer {
    public:
        Renderer(const char* application_name, const char* engine_name, SDL_Window* window_ptr);
        ~Renderer();
    private:
        Instance instance;
    };

}

#endif
