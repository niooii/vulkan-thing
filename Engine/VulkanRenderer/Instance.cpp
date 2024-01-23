//
// Created by beyon on 1/23/2024.
//

#include "Instance.h"

#include <iostream>

namespace VulkanRenderer {
    using std::vector;
    Instance::Instance(const char* application_name, const char* engine_name, SDL_Window* window_ptr) {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = application_name;
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = engine_name;
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledLayerCount = 0;

        // sdl extension query stuff
        uint32_t num_extensions;
        SDL_Vulkan_GetInstanceExtensions(window_ptr, &num_extensions, nullptr);
        vector<const char*> extension_names{num_extensions};
        SDL_Vulkan_GetInstanceExtensions(window_ptr, &num_extensions, extension_names.data());

        create_info.enabledExtensionCount = num_extensions;
        create_info.ppEnabledExtensionNames = extension_names.data();

        VkResult result = vkCreateInstance(&create_info, nullptr, &vk_instance_);

        if (result != VK_SUCCESS) {
            // TODO! log
            throw std::runtime_error(std::string("Failed to create instance! Error:") + string_VkResult(result));
        }
    }

    Instance::~Instance() {
        vkDestroyInstance(vk_instance_, nullptr);
    }
} // VulkanRenderer