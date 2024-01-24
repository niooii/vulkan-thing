#include "Device.h"

namespace Engine::Vulkan {

    Device::Device() {

    }

    void Device::Destroy() {
        vkDestroyDevice(vk_logical_, nullptr);
    }

    VkDevice Device::device() {
        return vk_logical_;
    }

}