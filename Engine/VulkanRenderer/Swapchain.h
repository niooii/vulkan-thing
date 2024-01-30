#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "Device.h"
#include "../Window.h"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace Engine::Vulkan {

    class Swapchain {
    public:
        Swapchain(Device &device, Instance &instance, Surface &surface, Window &window);
        void Destroy();

    private:
        Device& device_;
        Instance& instance_;
        Surface& surface_;
        Window& window_;

        VkSwapchainKHR vk_swapchain_;

        // Attributes
        VkExtent2D current_extent_;
        VkFormat image_format_;
        std::vector<VkImage> swapchain_images_;
        std::vector<VkImageView> swapchain_image_views_;

        // Internal
        VkSurfaceFormatKHR OptimalFormat();
        VkPresentModeKHR OptimalPresentMode();
        VkExtent2D GetSwapExtent();
        void CreateImages();
        void CreateImageViews();
    };

}

#endif
