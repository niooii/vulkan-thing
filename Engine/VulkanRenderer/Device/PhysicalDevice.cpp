#include "PhysicalDevice.h"

namespace Engine::Vulkan {

    PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_physical) : vk_physical_(vk_physical) {
        vkGetPhysicalDeviceProperties(vk_physical_, &vk_device_properties_);
    }

    std::string_view PhysicalDevice::name() {
        return std::string_view(vk_device_properties_.deviceName);
    }

}