#ifndef VULKAN_STUFF_DEVICE_H
#define VULKAN_STUFF_DEVICE_H

namespace Engine::Vulkan {

    class Device {
    public:
        Device();
        ~Device();
    private:
        VkPhysicalDevice physical_;
        VkDevice logical_;
    };

}

#endif
