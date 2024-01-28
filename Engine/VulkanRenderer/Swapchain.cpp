#include "Swapchain.h"

namespace Engine::Vulkan {

    Swapchain::Swapchain(Device &device, Instance &instance, Window &window) : device_(device), instance_(instance) {
        spdlog::debug("Vulkan swapchain has been initialized.");
    }

    void Swapchain::Destroy() {
        vkDestroySwapchainKHR(device_.vk_device(), vk_swapchain_, nullptr);
    }

}