#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

namespace Engine::Vulkan {

    class Device {
    public:
        // When creating devices, chooses the best device based off conditions that I will specify later.
        Device();
        void Destroy();

        // Accessors
        VkDevice device();
    private:
        VkPhysicalDevice vk_physical_;
        VkDevice vk_logical_;
    };

}

#endif
