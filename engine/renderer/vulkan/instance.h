#ifndef VULKAN_STUFF_INSTANCE_H
#define VULKAN_STUFF_INSTANCE_H

#include "window.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace Engine::Vulkan {

    class Instance {
    public:
        Instance(const char* application_name, const char* engine_name, Window &window, bool validation_layers_enabled);
        ~Instance();

        Instance(const Instance&) = delete;
        Instance& operator= (const Instance&) = delete;

        // Accessors
        inline VkInstance vk_instance() {
            return vk_instance_;
        }

    private:
        VkInstance vk_instance_;

        // Internal
        bool ValidationLayersSupported(std::vector<const char*>& validation_layers);
    };

}

#endif
