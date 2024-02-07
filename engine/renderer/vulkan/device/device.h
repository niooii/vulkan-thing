#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "../instance.h"
#include "../surface.h"
#include "utils.h"
#include "queue_family_indices.h"
#include "swapchain_supported_properties.h"
#include "physical_device.h"

#include "spdlog/spdlog.h"

#include <optional>
#include <set>

namespace Engine::Vulkan {

    class Device {
    public:
        Device(Instance &instance, Surface &surface, PhysicalDevice& physical_device);
        ~Device();


        Device(const Device&) = delete;
        Device& operator= (const Device&) = delete;

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
