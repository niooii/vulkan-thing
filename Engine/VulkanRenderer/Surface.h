#ifndef VULKAN_STUFF_SURFACE_H
#define VULKAN_STUFF_SURFACE_H

#include "Instance.h"

#include <vulkan/vulkan.h>
#include <SDL2/SDL_vulkan.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

namespace Engine::Vulkan {

    class Surface {
    public:
        Surface(Instance& instance, Window& window);
        void Destroy();

        // Accessors
        VkSurfaceKHR vk_surface();

    private:
        Instance& instance_;
        Window& window_;

        VkSurfaceKHR vk_surface_;

    };

}

#endif