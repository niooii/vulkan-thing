#include <stdexcept>
#include "Device.h"

namespace Engine::Vulkan {

    Device::Device(Instance &instance) {
        uint32_t num_devices;
        vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, nullptr);

        if(num_devices == 0) {
            throw std::runtime_error("failed to find device.");
        }

        std::vector<VkPhysicalDevice> physical_devices{num_devices};
        vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, physical_devices.data());

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

        // Create with given device create info

    }

    void Device::Destroy() {
        vkDestroyDevice(vk_logical_, nullptr);
    }

    VkDevice Device::device() {
        return vk_logical_;
    }

    // Internal
    bool Device::DeviceSuitable(VkPhysicalDevice physical_device) {
        // TODO! handle later, for now choose first found
        return true;
    }

    QueueFamilyIndicies Device::FindQueueFamilies(VkPhysicalDevice physical_device) {
        QueueFamilyIndicies indices;

        

        return indices;
    }

}