#ifndef VULKAN_STUFF_INSTANCE_H
#define VULKAN_STUFF_INSTANCE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <vector>

namespace Engine::Vulkan {
    using std::vector;
    class Instance {
    public:
        Instance(const char* application_name, const char* engine_name, SDL_Window* window_ptr, bool validation_layers_enabled);
        ~Instance();

    private:
        VkInstance vk_instance_;

        // Internal checks
        bool ValidationLayersSupported(vector<const char*> &validation_layers);
    };

}

#endif
