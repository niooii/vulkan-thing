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