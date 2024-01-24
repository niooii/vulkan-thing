#include "Instance.h"

#include <iostream>

namespace Engine::Vulkan {
    using std::vector;
    Instance::Instance(const char* application_name, const char* engine_name, SDL_Window* window_ptr, bool validation_layers_enabled) {
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

        // sdl extension query stuff
        uint32_t num_extensions;
        SDL_Vulkan_GetInstanceExtensions(window_ptr, &num_extensions, nullptr);
        vector<const char*> extension_names{num_extensions};
        SDL_Vulkan_GetInstanceExtensions(window_ptr, &num_extensions, extension_names.data());

        create_info.enabledExtensionCount = num_extensions;
        create_info.ppEnabledExtensionNames = extension_names.data();

        // handle validation layers
        vector<const char*> validation_layers = {
                "VK_LAYER_KHRONOS_validation"
        };
        if(validation_layers_enabled) {

            if(!ValidationLayersSupported(validation_layers)) {
                // TODO! log
                throw std::runtime_error(std::string("Failed to create instance_! Requested missing validation layers."));
            }

            create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            create_info.ppEnabledLayerNames = validation_layers.data();
        } else {
            create_info.enabledLayerCount = 0;
        }

        VkResult result = vkCreateInstance(&create_info, nullptr, &vk_instance_);

        if (result != VK_SUCCESS) {
            // TODO! log
            throw std::runtime_error(std::string("Failed to create instance_! Error: ") + string_VkResult(result));
        }
    }

    Instance::~Instance() {
        vkDestroyInstance(vk_instance_, nullptr);
    }

    // Internal checks
    bool Instance::ValidationLayersSupported(vector<const char*> &validation_layers) {
        uint32_t num_layers;
        vkEnumerateInstanceLayerProperties(&num_layers, nullptr);

        std::vector<VkLayerProperties> availableLayers{num_layers};
        vkEnumerateInstanceLayerProperties(&num_layers, availableLayers.data());

        for (const char* layerName : validation_layers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
}