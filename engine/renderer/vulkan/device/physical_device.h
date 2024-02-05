#ifndef VULKAN_STUFF_PHYSICAL_DEVICE_H
#define VULKAN_STUFF_PHYSICAL_DEVICE_H

#include <renderer/vulkan/surface.h>
#include "queue_family_indices.h"
#include "swapchain_supported_properties.h"

#include <set>

namespace Engine::Vulkan {

    class Device;

    class PhysicalDevice {
    public:
        explicit PhysicalDevice(VkPhysicalDevice vk_physical);
        // Accessors
        std::string_view name();
        VkPhysicalDevice vk_physical();
        const std::vector<const char*>& required_device_extensions();

        QueueFamilyIndices& get_queue_families(Surface& surface);
        SwapchainSupportedProperties& get_swapchain_support_properties(Surface& surface);
        bool HasRequiredExtensions();

        // modifying this to my own needs LOL
        bool IsSuitable(Surface& surface);

    private:
        std::optional<QueueFamilyIndices> queue_family_indices_;
        std::optional<SwapchainSupportedProperties> swapchain_support_properties_;

        VkPhysicalDevice vk_physical_;
        VkPhysicalDeviceProperties vk_device_properties_;

        // maybe pass this in or something later.
        const std::vector<const char*> required_device_extensions_ = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };

}


#endif
