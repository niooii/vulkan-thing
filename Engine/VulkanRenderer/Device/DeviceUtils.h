#ifndef VULKAN_STUFF_DEVICEUTILS_H
#define VULKAN_STUFF_DEVICEUTILS_H

#include <vector>
#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"
#include "VulkanRenderer/Instance.h"

namespace Engine::Vulkan {

    class DeviceUtils {
    public:
        static std::vector<PhysicalDevice> GetPhysicalDevices(Instance& instance) {
            uint32_t num_devices;
            vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, nullptr);
            std::vector<VkPhysicalDevice> vk_devices{num_devices};
            vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, vk_devices.data());

            std::vector<PhysicalDevice> physical_devices;
            auto f(VkphysicalDevice) -> PhysicalDevice(VkphysicalDevice)
            std::transform(vk_devices.front(), vk_devices.back(), std::back_inserter(physical_devices), [] {return PhysicalDevice(vk_physical)});

        }
    };

}

#endif