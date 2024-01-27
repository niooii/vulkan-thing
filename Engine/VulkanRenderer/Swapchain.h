#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "Device.h"
#include "../Window.h"

#include <stdexcept>

namespace Engine::Vulkan {

    class Swapchain {
    public:
        Swapchain(Device &device, Instance &instance, Window &window);
        // This will be called after vk_device finishes setting up.
        void Destroy();

    private:
        Device &device_;
        Instance &instance_;

        VkSwapchainKHR vk_swapchain_;

        // Internal
    };

}

#endif
