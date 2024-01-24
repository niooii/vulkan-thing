#include "Device.h"

namespace Engine::Vulkan {

    Device::Device(Instance &instance) {
        uint32_t num_devices;
        vkEnumeratePhysicalDevices(instance.instance(), &num_devices, nullptr);
    }

    void Device::Destroy() {
        vkDestroyDevice(vk_logical_, nullptr);
    }

    VkDevice Device::device() {
        return vk_logical_;
    }

}