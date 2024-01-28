#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "Instance.h"
#include "Surface.h"

#include <spdlog/spdlog.h>

#include <optional>

namespace Engine::Vulkan {

    struct QueueFamilyIndicies {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool all_exist() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    class Device {
    public:
        // When creating devices, chooses the best vk_device based off conditions that I will specify later.
        // TODO! this chooses the first found vk_device for now. implement better choosing later.
        Device(Instance &instance, Surface &surface);
        void Destroy();

        // Accessors
        VkDevice vk_device();

        // TODO! temporary, figure out what to do with this later
        VkQueue graphics_queue_handle;

    private:
        Instance &instance_;
        Surface &surface_;
        QueueFamilyIndicies queue_family_indices;

        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;

        // Internal
        void PickPhysicalDevice();
        void InitLogicalDevice();

        std::vector<VkDeviceQueueCreateInfo> GetQueueCreateInfos(QueueFamilyIndicies &indices);

        bool DeviceSuitable(VkPhysicalDevice physical_device);
        QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice physical_device);

    };

}

#endif
