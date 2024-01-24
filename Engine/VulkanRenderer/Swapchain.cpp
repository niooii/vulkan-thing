#include "Swapchain.h"

namespace Engine::Vulkan {

    Swapchain::Swapchain(Device &device) : device_(device) {

    }

    void Swapchain::Destroy() {
        vkDestroySwapchainKHR(device_.device(), vk_swapchain_, nullptr);
    }

}