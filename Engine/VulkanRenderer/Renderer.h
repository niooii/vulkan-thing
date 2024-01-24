#ifndef VULKAN_STUFF_RENDERER_H
#define VULKAN_STUFF_RENDERER_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "Device.h"

#include <vector>

namespace Engine::Vulkan {

    class Renderer {
    public:
        // The parameters passed in are used to create the underlying Vulkan instance_.
        Renderer(const char* application_name, const char* engine_name, SDL_Window* window_ptr, bool validation_layers_enabled);
        ~Renderer();
    private:

        Instance instance_;
        Device device_;
    };

}

#endif
