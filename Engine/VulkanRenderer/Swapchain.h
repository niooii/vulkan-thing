#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "Device.h"

namespace Engine::Vulkan {

    class Swapchain {
    public:
        Swapchain(Device &device);
        void Destroy();
    private:
        VkSwapchainKHR vk_swapchain_;
        Device &device_;
    };

}


#endif
