#include "frame_sync.h"

namespace Engine::Vulkan {

    FrameSync::FrameSync(Device& device) : device_(device) {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        Utils::ExpectBadResult("Failed to create semaphore", vkCreateSemaphore(
                device.vk_device(), &semaphore_info, nullptr, &vk_image_available_semaphore_
        ));
        Utils::ExpectBadResult("Failed to create semaphore", vkCreateSemaphore(
                device.vk_device(), &semaphore_info, nullptr, &vk_render_finished_semaphore_
        ));
        Utils::ExpectBadResult("Failed to create fence", vkCreateFence(
                device.vk_device(), &fence_info, nullptr, &vk_inflight_fence_
        ));
    }

    FrameSync::~FrameSync() {
        vkDestroySemaphore(
                device_.vk_device(),
                vk_image_available_semaphore_,
                nullptr
        );
        vkDestroySemaphore(
                device_.vk_device(),
                vk_render_finished_semaphore_,
                nullptr
        );
        vkDestroyFence(
                device_.vk_device(),
                vk_inflight_fence_,
                nullptr
        );
    }

}