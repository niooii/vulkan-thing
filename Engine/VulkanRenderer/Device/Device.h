#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "VulkanRenderer/Instance.h"
#include "VulkanRenderer/Surface.h"
#include "Utils.h"
#include "QueueFamilyIndices.h"
#include "SwapchainSupportedProperties.h"
#include "PhysicalDevice.h"

#include "spdlog/spdlog.h"

#include <optional>
#include <set>

namespace Engine::Vulkan {

    class Device {
    public:
        Device(Instance &instance, Surface &surface, PhysicalDevice& physical_device);
        void Destroy();

        // Accessors
        VkDevice vk_device();
        QueueFamilyIndices& get_queue_families();
        SwapchainSupportedProperties& get_swapchain_support_properties();

        // TODO! temporary, figure out what to do with this later
        VkQueue graphics_queue_handle;
        VkQueue present_queue_handle;

    private:
        Instance& instance_;
        Surface& surface_;
        QueueFamilyIndices queue_family_indices_;
        SwapchainSupportedProperties sc_supported_properties_;

//        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;
        std::vector<VkDeviceQueueCreateInfo> GetQueueCreateInfos(QueueFamilyIndices &indices);
    };

}

#endif
