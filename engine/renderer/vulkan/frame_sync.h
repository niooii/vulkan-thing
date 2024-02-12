#ifndef VULKAN_STUFF_FRAME_SYNC_H
#define VULKAN_STUFF_FRAME_SYNC_H


#include <renderer/vulkan/device/device.h>

namespace Engine::Vulkan {

    class FrameSync {
    public:
        explicit FrameSync(Device& device);
        ~FrameSync();
        inline VkSemaphore image_available_semaphore() {
            return vk_image_available_semaphore_;
        }
        inline VkSemaphore render_finished_semaphore() {
            return vk_render_finished_semaphore_;
        }
        inline VkFence in_flight_fence() {
            return vk_inflight_fence_;
        }
    private:
        Device& device_;

        VkSemaphore vk_image_available_semaphore_;
        VkSemaphore vk_render_finished_semaphore_;
        VkFence vk_inflight_fence_;
    };

};


#endif