#include "command_buffer_recorder.h"

namespace Engine::Vulkan {

    CommandBufferRecorder::CommandBufferRecorder(Device& device, Swapchain& swapchain, GraphicsPipeline& graphics_pipeline)
    : device_(device), swapchain_(swapchain), graphics_pipeline_(graphics_pipeline){
        QueueFamilyIndices queue_indices = device.get_queue_families();

        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = queue_indices.graphics_family.value();

        Utils::ExpectBadResult("Could not create cmd pool", vkCreateCommandPool(device.vk_device(), &pool_info, nullptr, &vk_cmd_pool_));

        VkCommandBufferAllocateInfo buffer_alloc_info{};
        buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        buffer_alloc_info.commandBufferCount = 1;
        buffer_alloc_info.commandPool = vk_cmd_pool_;
        buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        Utils::ExpectBadResult("Failed to allocate command buffers", vkAllocateCommandBuffers(device.vk_device(), &buffer_alloc_info, &vk_cmd_buffer_));

        spdlog::debug("Vulkan cmd buffer recorder initialized.");
    }

    CommandBufferRecorder::~CommandBufferRecorder() {
        vkDestroyCommandPool(device_.vk_device(), vk_cmd_pool_, nullptr);
        spdlog::debug("Vulkan cmd buffer recorder destroyed.");
    }

    void CommandBufferRecorder::RecordCommandBuffers(uint32_t image_index) {
        VkCommandBufferBeginInfo cmd_begin_info{};
        cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_begin_info.flags = 0;
        cmd_begin_info.pInheritanceInfo = nullptr; // Optional

        Utils::ExpectBadResult("Failed to start command buffer", vkBeginCommandBuffer(vk_cmd_buffer_, &cmd_begin_info));

        VkRenderPassBeginInfo rp_info{};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.renderPass = graphics_pipeline_.render_pass();
        rp_info.framebuffer = swapchain_.vk_frame_buffers()[image_index];
        rp_info.renderArea.offset = {0, 0};
        rp_info.renderArea.extent = swapchain_.vk_extent();

        VkClearValue clear_color {
            {
                {
                    0.0f, 0.0f, 0.0f, 1.0f
                }
            }
        };

        rp_info.clearValueCount = 1;
        rp_info.pClearValues = &clear_color;

        vkCmdBeginRenderPass(vk_cmd_buffer_, &rp_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(vk_cmd_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_.vk_pipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchain_.vk_extent().width);
        viewport.height = static_cast<float>(swapchain_.vk_extent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(vk_cmd_buffer_, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain_.vk_extent();
        vkCmdSetScissor(vk_cmd_buffer_, 0, 1, &scissor);

        vkCmdDraw(vk_cmd_buffer_, 3, 1, 0, 0);

        vkCmdEndRenderPass(vk_cmd_buffer_);

        Utils::ExpectBadResult("Failed to end command buffer", vkEndCommandBuffer(vk_cmd_buffer_));
    }

    void CommandBufferRecorder::ResetCommandBuffers() {
        Utils::ExpectBadResult(
                "Failed to reset command buffers",
                vkResetCommandBuffer(vk_cmd_buffer_, 0)
                );
    }

}