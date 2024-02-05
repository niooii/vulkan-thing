#ifndef VULKAN_STUFF_DEVICE_UTILS_H
#define VULKAN_STUFF_DEVICE_UTILS_H

#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include "physical_device.h"
#include "../instance.h"

namespace Engine::Vulkan {

    namespace DeviceUtils {
        static std::vector<PhysicalDevice> GetPhysicalDevices(Instance& instance) {
            uint32_t num_devices;
            vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, nullptr);
            std::vector<VkPhysicalDevice> vk_devices{num_devices};
            vkEnumeratePhysicalDevices(instance.vk_instance(), &num_devices, vk_devices.data());

            std::vector<PhysicalDevice> physical_devices{};

            for(const auto& vk_device : vk_devices) {
                physical_devices.emplace_back(vk_device);
            }

            return physical_devices;
        }


    };

}

#endif