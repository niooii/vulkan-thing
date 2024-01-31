#ifndef VULKAN_STUFF_INSTANCE_H
#define VULKAN_STUFF_INSTANCE_H

#include "../Window.h"
#include "Utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace Engine::Vulkan {

    class Instance {
    public:
        Instance(const char* application_name, const char* engine_name, Window &window, bool validation_layers_enabled);
        void Destroy();

        VkInstance vk_instance();

    private:
        VkInstance vk_instance_;

        // Internal
        bool ValidationLayersSupported(std::vector<const char*>& validation_layers);
    };

}

#endif
