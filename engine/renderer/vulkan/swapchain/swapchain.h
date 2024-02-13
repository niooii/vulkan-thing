#ifndef VULKAN_STUFF_SWAPCHAIN_H
#define VULKAN_STUFF_SWAPCHAIN_H

#include "renderer/vulkan/device/device.h"
#include "window.h"
#include "utils.h"
#include "renderer/vulkan/graphics_pipeline.h"

#include <spdlog/spdlog.h>

#include <stdexcept>
#include <renderer/vulkan/frame_sync.h>

namespace Engine::Vulkan {

    class Swapchain {
    public:
        // Set width and height based on window and device specs
        Swapchain(Device &device, Instance &instance, Surface &surface, Window &window);
        ~Swapchain();

        Swapchain(const Swapchain&) = delete;
        Swapchain& operator= (const Swapchain&) = delete;

        // TODO! Explicitly set width and height

        // Accessors
        inline VkSwapchainKHR vk_swapchain() {
            return vk_swapchain_;
        }
        inline VkExtent2D vk_extent() {
            return vk_extent_;
        }
        inline VkFormat vk_image_format() {
            return vk_image_format_;
        }
        inline std::vector<VkImageView>& vk_image_views() {
            return swapchain_image_views_;
        }
        inline std::vector<VkFramebuffer>& vk_frame_buffers() {
            return swapchain_frame_buffers_;
        }
        inline FrameSync& frame_sync() {
            return frame_syncs[0];
        }

        // initializes framebuffers
        void CreateFramebuffers(VkRenderPass render_pass);
        uint32_t AcquireNextImageIdx();
        void Present(VkSemaphore render_complete_semaphore, uint32_t image_idx);
        bool Resize();


    private:
        Device& device_;
        Instance& instance_;
        Surface& surface_;
        Window& window_;

        VkSwapchainKHR vk_swapchain_;

        // Attributes
        VkExtent2D vk_extent_;
        VkFormat vk_image_format_;
        std::vector<VkImage> swapchain_images_;
        std::vector<VkImageView> swapchain_image_views_;
        std::vector<VkFramebuffer> swapchain_frame_buffers_;
        std::vector<FrameSync> frame_syncs;
        uint32_t max_frames_in_flight_ = 2;

        // Internal
        void Recreate(bool use_window_dims);
        VkSurfaceFormatKHR OptimalFormat();
        VkPresentModeKHR OptimalPresentMode();
        VkExtent2D GetSwapExtent();
        void CreateImages();
        void CreateImageViews();
    };

}

#endif
