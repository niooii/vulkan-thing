#ifndef VULKAN_STUFF_SURFACE_H
#define VULKAN_STUFF_SURFACE_H

#include "instance.h"
#include "utils.h"

#include <vulkan/vulkan.h>
#include <SDL2/SDL_vulkan.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

namespace Engine::Vulkan {

    class Surface {
    public:
        Surface(Instance& instance, Window& window);
        ~Surface();

        Surface(const Surface&) = delete;
        Surface& operator= (const Surface&) = delete;

        // Accessors
        VkSurfaceKHR vk_surface();

    private:
        Instance& instance_;
        Window& window_;

        VkSurfaceKHR vk_surface_;

    };

}

#endif