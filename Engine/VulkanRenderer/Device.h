#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "Instance.h"

#include <optional>

namespace Engine::Vulkan {

    struct QueueFamilyIndicies {
        std::optional<uint32_t> graphics_family;

    };

    class Device {
    public:
        // When creating devices, chooses the best device based off conditions that I will specify later.
        // TODO! this chooses the first found device for now. implement better choosing later.
        explicit Device(Instance &instance);
        void Destroy();

        // Accessors
        VkDevice device();
    private:
        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;

        // Internal
        bool DeviceSuitable(VkPhysicalDevice physical_device);
        QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice physical_device);

    };

}

#endif
