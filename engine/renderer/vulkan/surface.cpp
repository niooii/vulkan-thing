#include "surface.h"

namespace Engine::Vulkan {

    Surface::Surface(Instance& instance, Window& window)
    : instance_(instance), window_(window) {
        SDL_bool res = SDL_Vulkan_CreateSurface(window.sdl_window(), instance.vk_instance(), &vk_surface_);

        if(res == SDL_FALSE) {
            throw std::runtime_error("Failed to create vulkan surface.");
        }
        spdlog::debug("Vulkan surface has been initialized.");
    }

    Surface::~Surface() {
        vkDestroySurfaceKHR(instance_.vk_instance(), vk_surface_,   nullptr);
    }

}