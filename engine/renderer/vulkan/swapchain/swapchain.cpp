#include "swapchain.h"

namespace Engine::Vulkan {

    Swapchain::Swapchain(Device& device, Instance& instance, Surface& surface, Window& window)
    : device_(device), instance_(instance), surface_(surface), window_(window) {

        const auto& surface_capabilities = device.get_swapchain_support_properties().capabilities;

        VkSurfaceFormatKHR optimal_format = OptimalFormat();

        vk_extent_ = GetSwapExtent();
        vk_image_format_ = optimal_format.format;

        VkSwapchainCreateInfoKHR sc_create_info{};

        sc_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        sc_create_info.imageFormat = vk_image_format_;
        sc_create_info.imageColorSpace = optimal_format.colorSpace;
        sc_create_info.presentMode = OptimalPresentMode();
        sc_create_info.imageExtent = vk_extent_;

        sc_create_info.surface = surface.vk_surface();

        bool has_no_max = surface_capabilities.maxImageCount > 0;
        uint32_t image_count = surface_capabilities.minImageCount + 1;
        // if there is a max and the img count is above the max, clamp down to max.
        sc_create_info.minImageCount = (!has_no_max && image_count > surface_capabilities.maxImageCount)
                ? surface_capabilities.maxImageCount : image_count;

        // mostly 1 unless some weird 3d thing
        sc_create_info.imageArrayLayers = 1;
        // color attachment bc directly render
        sc_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices& q_indices = device.get_queue_families();
        uint32_t all_indices[] = {
                q_indices.graphics_family.value(), q_indices.present_family.value()
        };

        if (q_indices.graphics_family != q_indices.present_family) {
            sc_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            sc_create_info.queueFamilyIndexCount = 2;
            sc_create_info.pQueueFamilyIndices = all_indices;
        } else {
            sc_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            sc_create_info.queueFamilyIndexCount = 0; // Optional
            sc_create_info.pQueueFamilyIndices = nullptr; // Optional
        }

        sc_create_info.preTransform = surface_capabilities.currentTransform;
        // specifies blending with other windows
        sc_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        // other windows on top blah blah
        sc_create_info.clipped = VK_TRUE;

        // handle later
        sc_create_info.oldSwapchain = VK_NULL_HANDLE;

        VkResult res = vkCreateSwapchainKHR(device.vk_device(), &sc_create_info, nullptr, &vk_swapchain_);

        if(res != VK_SUCCESS) {
            spdlog::error("swapchain initialize error: {}", string_VkResult(res));
            throw std::runtime_error("Failed to create swapchain.");
        }

        spdlog::debug("Vulkan swapchain has been initialized.");

        CreateImages();
        CreateImageViews();

        // TODO! refactor later
        frame_syncs.emplace_back(device_);
    }

    Swapchain::~Swapchain() {
        for(const auto& image_view : swapchain_image_views_) {
            vkDestroyImageView(device_.vk_device(), image_view, nullptr);
        }
        for(const auto& frame_buffer : swapchain_frame_buffers_) {
            vkDestroyFramebuffer(device_.vk_device(), frame_buffer, nullptr);
        }

        vkDestroySwapchainKHR(device_.vk_device(), vk_swapchain_, nullptr);

        spdlog::debug("Vulkan swapchain destroyed.");
    }

    void Swapchain::CreateFramebuffers(VkRenderPass render_pass) {
        swapchain_frame_buffers_.resize(swapchain_image_views_.size());

        for (size_t i = 0; i < swapchain_image_views_.size(); i++) {
            VkImageView attachments[] = {
                    swapchain_image_views_[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = render_pass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = vk_extent_.width;
            framebufferInfo.height = vk_extent_.height;
            framebufferInfo.layers = 1;

            Utils::ExpectBadResult("Failed to create framebuffers.", vkCreateFramebuffer(device_.vk_device(), &framebufferInfo, nullptr, &swapchain_frame_buffers_[i]));
        }
    }

    uint32_t Swapchain::AcquireNextImageIdx() {
        uint32_t idx;

        VkResult result = vkAcquireNextImageKHR(device_.vk_device(), vk_swapchain_, 3000000000000,
                                       frame_syncs[0].image_available_semaphore(), VK_NULL_HANDLE, &idx);

        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            Recreate(true);
        }
        else if(result != VK_SUCCESS) {
            throw std::runtime_error("Failed to acquire next image idx.");
        }

        return idx;
    }

    void Swapchain::Present(VkSemaphore render_complete_semaphore, uint32_t image_idx) {
        VkSemaphore signal_semaphores[] = {render_complete_semaphore};

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swapchains[] = {vk_swapchain_};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &image_idx;

        present_info.pResults = nullptr;

        vkQueuePresentKHR(device_.present_queue_handle, &present_info);
    }
    
    // Internal
    VkSurfaceFormatKHR Swapchain::OptimalFormat() {
        const auto &available_formats = device_.get_swapchain_support_properties().formats;

        for(const auto &format : available_formats) {
//            spdlog::debug("{} {}", string_VkFormat(format.format), string_VkColorSpaceKHR(format.colorSpace));

            // apparnetly srgb leads to more accurate colors:
            // https://stackoverflow.com/questions/12524623/what-are-the-practical-differences-when-working-with-colors-in-a-linear-vs-a-no
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return available_formats[0];
    }

    VkPresentModeKHR Swapchain::OptimalPresentMode() {
        const auto &available_present_modes = device_.get_swapchain_support_properties().present_modes;

        for(const auto &present_mode : available_present_modes) {
//            spdlog::debug("{}", string_VkPresentModeKHR(present_mode));

            if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return present_mode;
            }
        }

        // this is guarenteed to be available.
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::GetSwapExtent() {
        const VkSurfaceCapabilitiesKHR& surface_capabilities = device_.get_swapchain_support_properties().capabilities;

        if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return surface_capabilities.currentExtent;
        }

        // should be in pixels already from sdl docs and stuff
        uint32_t width = static_cast<uint32_t>(window_.width());
        uint32_t height = static_cast<uint32_t>(window_.height());

        VkExtent2D extent{
            width,
            height
        };

        extent.width = std::clamp(
                extent.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width
                );
        extent.height = std::clamp(
                extent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height
                );

        return extent;
    }

    void Swapchain::CreateImages() {
    	uint32_t num_images;
    	vkGetSwapchainImagesKHR(device_.vk_device(), vk_swapchain_, &num_images, nullptr);
    	swapchain_images_.resize(num_images);
    	vkGetSwapchainImagesKHR(device_.vk_device(), vk_swapchain_, &num_images, swapchain_images_.data());
    }

    void Swapchain::CreateImageViews() {
        swapchain_image_views_.resize(swapchain_images_.size());

        // prob using the same info for everything
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = vk_image_format_;

        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        for(size_t i = 0; i < swapchain_images_.size(); i++) {
            create_info.image = swapchain_images_[i];
            VkResult result = vkCreateImageView(device_.vk_device(), &create_info, nullptr, &swapchain_image_views_[i]);

            Utils::ExpectBadResult("Failed to create imageview", result);
        }
    }



}
