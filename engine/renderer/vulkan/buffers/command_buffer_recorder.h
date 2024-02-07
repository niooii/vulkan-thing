#ifndef VULKAN_STUFF_COMMAND_BUFFER_RECORDER_H
#define VULKAN_STUFF_COMMAND_BUFFER_RECORDER_H

#include "renderer/vulkan/device/device.h"
#include "renderer/vulkan/swapchain/swapchain.h"
#include "renderer/vulkan/graphics_pipeline.h"

namespace Engine::Vulkan {

    class CommandBufferRecorder {
    public:
        CommandBufferRecorder(Device& device, Swapchain& swapchain, GraphicsPipeline& graphics_pipeline);
        ~CommandBufferRecorder();

        CommandBufferRecorder(const CommandBufferRecorder&) = delete;
        CommandBufferRecorder& operator= (const CommandBufferRecorder&) = delete;

        // Record and submit command buffers
        void RecordCommandBuffers(uint32_t image_index);

    private:
        Device& device_;
        Swapchain& swapchain_;
        GraphicsPipeline& graphics_pipeline_;

        VkCommandPool vk_cmd_pool_;
        VkCommandBuffer vk_buffer;

    };

}

#endif
