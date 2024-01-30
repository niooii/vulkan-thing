#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "Instance.h"
#include "Surface.h"

#include <spdlog/spdlog.h>

#include <optional>
#include <set>

namespace Engine::Vulkan {

    struct QueueFamilyIndicies {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool all_exist() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class Device {
    public:
        // When creating devices, chooses the best vk_device based off conditions that I will specify later.
        // TODO! this chooses the first found vk_device for now. implement better choosing later.
        Device(Instance &instance, Surface &surface);
        void Destroy();

        // Accessors
        VkDevice vk_device();
        SwapChainSupportDetails &swapchain_support_details();
//        VkPhysicalDevice vk_physical();

        // TODO! temporary, figure out what to do with this later
        VkQueue graphics_queue_handle;
        VkQueue present_queue_handle;

    private:
        Instance &instance_;
        Surface &surface_;
        QueueFamilyIndicies queue_family_indices_;
        SwapChainSupportDetails swapchain_support_details_;

        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;

        // Internal
        void PickPhysicalDevice();
        void InitLogicalDevice();

        std::vector<VkDeviceQueueCreateInfo> GetQueueCreateInfos(QueueFamilyIndicies &indices);

        const std::vector<const char*> required_device_extensions_ = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        bool DeviceSuitable(VkPhysicalDevice physical_device);
        bool HasRequiredExtensions(VkPhysicalDevice physical_device);
        QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice physical_device);
        SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physical_device);

    };

}

#endif
