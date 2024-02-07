#include <stdexcept>
#include "device.h"

namespace Engine::Vulkan {

    Device::Device(Instance &instance, Surface &surface, PhysicalDevice& physical_device) : instance_(instance), surface_(surface) {
        // how to explicitly copy i forgot wahhh
        queue_family_indices_ = physical_device.get_queue_families(surface);
        sc_supported_properties_ = physical_device.get_swapchain_support_properties(surface);

        VkDeviceCreateInfo dev_create_info{};
        dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos = GetQueueCreateInfos(queue_family_indices_);

        dev_create_info.pQueueCreateInfos = queue_create_infos.data();
        dev_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

        // empty for now
        VkPhysicalDeviceFeatures dev_features{};

        dev_create_info.pEnabledFeatures = &dev_features;

        // device extensions

        dev_create_info.ppEnabledExtensionNames = physical_device.required_device_extensions().data();
        dev_create_info.enabledExtensionCount = static_cast<uint32_t>(
                physical_device.required_device_extensions().size()
        );

        VkResult result = vkCreateDevice(physical_device.vk_physical(), &dev_create_info, nullptr, &vk_logical_);

        Utils::ExpectBadResult("Failed to create vk_device", result);

        vkGetDeviceQueue(vk_logical_, queue_family_indices_.graphics_family.value(),
                         0, &graphics_queue_handle);

        vkGetDeviceQueue(vk_logical_, queue_family_indices_.present_family.value(),
                         0, &present_queue_handle);
        spdlog::debug("Vulkan device has been initialized.");
    }

    Device::~Device() {
        vkDestroyDevice(vk_logical_, nullptr);
        spdlog::debug("Vulkan device destroyed.");
    }

    VkDevice Device::vk_device() {
        return vk_logical_;
    }

    QueueFamilyIndices& Device::get_queue_families() {
        return queue_family_indices_;
    }

    SwapchainSupportedProperties& Device::get_swapchain_support_properties() {
        return sc_supported_properties_;
    }

    std::vector<VkDeviceQueueCreateInfo> Device::GetQueueCreateInfos(QueueFamilyIndices &indices) {
        VkDeviceQueueCreateInfo graphics_create_info{};
        graphics_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphics_create_info.queueFamilyIndex = indices.graphics_family.value();
        graphics_create_info.queueCount = 1;

        float graphic_q_prio = 1.0f;
        graphics_create_info.pQueuePriorities = &graphic_q_prio;

        VkDeviceQueueCreateInfo present_create_info{};
        present_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        present_create_info.queueFamilyIndex = indices.present_family.value();
        present_create_info.queueCount = 1;

        float present_q_prio = 1.0f;
        present_create_info.pQueuePriorities = &present_q_prio;

        return std::vector<VkDeviceQueueCreateInfo> {
            graphics_create_info,
            present_create_info
        };
    }

}