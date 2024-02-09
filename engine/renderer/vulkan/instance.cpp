#include "instance.h"

#include <iostream>

namespace Engine::Vulkan {

    Instance::Instance(const char* application_name, const char* engine_name, Window& window, bool debug) {
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

        // extensions
        uint32_t num_surface_exts;
        SDL_Vulkan_GetInstanceExtensions(window.sdl_window(), &num_surface_exts, nullptr);
        std::vector<const char*> extension_names{num_surface_exts};
        SDL_Vulkan_GetInstanceExtensions(window.sdl_window(), &num_surface_exts, extension_names.data());

        extension_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        create_info.enabledExtensionCount = extension_names.size();
        create_info.ppEnabledExtensionNames = extension_names.data();

        // handle validation layers
        std::vector<const char*> validation_layers = {
                "VK_LAYER_KHRONOS_validation"
        };
        if(debug) {

            if(!ValidationLayersSupported(validation_layers)) {
                // TODO! log
                throw std::runtime_error(std::string("Failed to create instance! Requested missing validation layers."));
            }

            create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            create_info.ppEnabledLayerNames = validation_layers.data();

        } else {
            create_info.enabledLayerCount = 0;
        }

        VkResult result = vkCreateInstance(&create_info, nullptr, &vk_instance_);

        Utils::ExpectBadResult("Failed to create vulkan instance", result);

        spdlog::debug("Vulkan instance has been initialized.");

        if(debug) {
            spdlog::debug("Creating debug messenger.");
            uint32_t log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
//                                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

            VkDebugUtilsMessengerCreateInfoEXT messenger_info{};
            messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            messenger_info.messageSeverity = log_severity;
            messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
            messenger_info.pfnUserCallback = MessengerCallback;
            auto create_func =
                    (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                            vk_instance_,
                            "vkCreateDebugUtilsMessengerEXT"
                    );

            assert(create_func != nullptr);

            Utils::ExpectBadResult(
                    "Failed to create debug messenger.",
                    create_func(vk_instance_, &messenger_info, nullptr, &vk_debug_messenger_)
            );

            spdlog::debug("Created debug messenger.");
        }
    }

    Instance::~Instance() {
        if(vk_debug_messenger_ != VK_NULL_HANDLE) {
            auto destroy_func =
                    (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                            vk_instance_,
                            "vkDestroyDebugUtilsMessengerEXT");

            assert(destroy_func != nullptr);
            destroy_func(vk_instance_, vk_debug_messenger_, nullptr);
        }
        vkDestroyInstance(vk_instance_, nullptr);
        spdlog::debug("Vulkan instance destroyed.");
    }

    // Internal
    bool Instance::ValidationLayersSupported(std::vector<const char*>& validation_layers) {
        uint32_t num_layers;
        vkEnumerateInstanceLayerProperties(&num_layers, nullptr);

        std::vector<VkLayerProperties> available_layers{num_layers};
        vkEnumerateInstanceLayerProperties(&num_layers, available_layers.data());

        for (const char* layer_name : validation_layers) {
            bool found_layer = false;

            for (const auto& layer_properties : available_layers) {
                if (strcmp(layer_name, layer_properties.layerName) == 0) {
                    found_layer = true;
                    break;
                }
            }

            if (!found_layer) {
                return false;
            }
        }

        return true;
    }

}