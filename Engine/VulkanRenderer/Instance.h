//
// Created by beyon on 1/23/2024.
//

#ifndef VULKAN_STUFF_INSTANCE_H
#define VULKAN_STUFF_INSTANCE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace VulkanRenderer {

    class Instance {
    public:
        Instance(const char* application_name, const char* engine_name, SDL_Window* window_ptr);
        ~Instance();

    private:
        VkInstance vk_instance_;
    };

}

#endif
