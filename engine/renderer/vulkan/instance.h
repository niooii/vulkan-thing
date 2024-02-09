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
        Instance(const char* application_name, const char* engine_name, Window &window, bool debug);
        ~Instance();

        Instance(const Instance&) = delete;
        Instance& operator= (const Instance&) = delete;

        // Accessors
        inline VkInstance vk_instance() {
            return vk_instance_;
        }

    private:
        VkInstance vk_instance_;
        VkDebugUtilsMessengerEXT vk_debug_messenger_{VK_NULL_HANDLE};

        // Internal
        bool ValidationLayersSupported(std::vector<const char*>& validation_layers);
        static VKAPI_ATTR VkBool32 VKAPI_CALL MessengerCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                VkDebugUtilsMessageTypeFlagsEXT message_types,
                const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                void* user_data) {
            switch(message_severity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                    spdlog::trace("{}", callback_data->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    spdlog::info("{}", callback_data->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    spdlog::warn("{}", callback_data->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    spdlog::error("{}", callback_data->pMessage);
                    break;
            }
        // according to the spec or something
        return VK_FALSE;
        }

    };

}

#endif
