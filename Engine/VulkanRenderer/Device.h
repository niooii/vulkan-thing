#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

#include "Instance.h"

namespace Engine::Vulkan {

    class Device {
    public:
        // When creating devices, chooses the best device based off conditions that I will specify later.
        // TODO! this chooses the first found device for now. implement better choosing later.
        Device(Instance &instance);
        void Destroy();

        // Accessors
        VkDevice device();
    private:
        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;
    };

}

#endif
