#ifndef VULKAN_STUFF_RENDERER_H
#define VULKAN_STUFF_RENDERER_H

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "Device.h"
#include "Swapchain.h"
#include "Surface.h"
#include <vector>
#include <spdlog.h>

namespace Engine::Vulkan {

    class Renderer {
    public:
        // The parameters passed in are used to create the underlying Vulkan instance_.
        Renderer(const char* application_name, const char* engine_name, Window &window, bool validation_layers_enabled);
        // Destroys all resources in *deliberate* order.
        ~Renderer();
    private:
        Instance instance_;
        Surface surface_;
        Device device_;
        Swapchain swapchain_;
    };

}

#endif
