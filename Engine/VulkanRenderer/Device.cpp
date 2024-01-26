#include <stdexcept>
#include "Device.h"

namespace Engine::Vulkan {

    Device::Device(Instance &instance) : instance_(instance) {
        PickPhysicalDevice();
        InitLogicalDevice();
    }

    void Device::Destroy() {
        vkDestroyDevice(vk_logical_, nullptr);
    }

    VkDevice Device::device() {
        return vk_logical_;
    }

    void Device::PickPhysicalDevice() {
        uint32_t num_devices;
        vkEnumeratePhysicalDevices(instance_.vk_instance(), &num_devices, nullptr);

        if(num_devices == 0) {
            throw std::runtime_error("failed to find device.");
        }

        std::vector<VkPhysicalDevice> physical_devices{num_devices};
        vkEnumeratePhysicalDevices(instance_.vk_instance(), &num_devices, physical_devices.data());

        bool found_suitable_device{false};
        for(auto physical : physical_devices) {
            if(DeviceSuitable(physical)) {
                found_suitable_device = true;
                vk_physical_ = physical;
            }
        }

        if(!found_suitable_device) {
            throw std::runtime_error("failed to find suitable device.");
        }

        queue_family_indices = FindQueueFamilies(vk_physical_);
        if(!queue_family_indices.all_exist()) {
            throw std::runtime_error("device is missing required queue families.");
        }
    }

    void Device::InitLogicalDevice() {
        // TODO!
        VkDeviceCreateInfo dev_create_info{};
        dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos = GetQueueCreateInfos(queue_family_indices);

        dev_create_info.pQueueCreateInfos = queue_create_infos.data();
        dev_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

        // empty for now
        VkPhysicalDeviceFeatures dev_features{};

        dev_create_info.pEnabledFeatures = &dev_features;

        VkResult result = vkCreateDevice(vk_physical_, &dev_create_info, nullptr, &vk_logical_);

        if(result != VK_SUCCESS) {
            // TODO! log
            throw std::runtime_error(std::string("Failed to create device! Error: ") + string_VkResult(result));
        }
    }

    std::vector<VkDeviceQueueCreateInfo> Device::GetQueueCreateInfos(QueueFamilyIndicies &indices) {
        VkDeviceQueueCreateInfo graphics_create_info{};
        graphics_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphics_create_info.queueFamilyIndex = indices.graphics_family.value();
        graphics_create_info.queueCount = 1;

        float q_prio = 1.0f;
        graphics_create_info.pQueuePriorities = &q_prio;

        return std::vector<VkDeviceQueueCreateInfo> {
            graphics_create_info
        };
    }

    // Internal
    bool Device::DeviceSuitable(VkPhysicalDevice physical_device) {
        // TODO! handle later, for now choose first found
        return true;
    }

    QueueFamilyIndicies Device::FindQueueFamilies(VkPhysicalDevice physical_device) {
        QueueFamilyIndicies indices;

        uint32_t num_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_families, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_families, queue_families.data());

        uint32_t i{0};
        for (const auto& queueFamily : queue_families) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }

            i++;
        }

        return indices;
    }

}