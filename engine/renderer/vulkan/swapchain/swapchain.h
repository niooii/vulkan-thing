#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "renderer/vulkan/device/device.h"
#include "window.h"
#include "utils.h"
#include "renderer/vulkan/graphics_pipeline.h"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace Engine::Vulkan {

    class Swapchain {
    public:
        // Set width and height based on window and device specs
        Swapchain(Device &device, Instance &instance, Surface &surface, Window &window);
        void Destroy();

        Swapchain(const Swapchain&) = delete;
        Swapchain& operator= (const Swapchain&) = delete;

        // TODO! Explicitly set width and height

        // Accessors
        VkExtent2D extent();
        VkFormat image_format();
        std::vector<VkImageView>& image_views();

        // initializes framebuffers
        void CreateFramebuffers(VkRenderPass render_pass);
        bool AcquireNextImage();
        bool Resize();


    private:
        Device& device_;
        Instance& instance_;
        Surface& surface_;
        Window& window_;

        VkSwapchainKHR vk_swapchain_;

        // Attributes
        VkExtent2D extent_;
        VkFormat image_format_;
        std::vector<VkImage> swapchain_images_;
        std::vector<VkImageView> swapchain_image_views_;
        std::vector<VkFramebuffer> swapchain_frame_buffers_;

        // Internal
        void InitSwapchain(bool use_window_dims);
        VkSurfaceFormatKHR OptimalFormat();
        VkPresentModeKHR OptimalPresentMode();
        VkExtent2D GetSwapExtent();
        void CreateImages();
        void CreateImageViews();
    };

}

#endif
