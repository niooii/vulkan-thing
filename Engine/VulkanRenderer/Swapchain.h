#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "Device.h"
#include "../Window.h"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace Engine::Vulkan {

    class Swapchain {
    public:
        Swapchain(Device &device, Instance &instance, Window &window);
        void Destroy();

    private:
        Device &device_;
        Instance &instance_;

        VkSwapchainKHR vk_swapchain_;

        // Internal
    };

}

#endif
