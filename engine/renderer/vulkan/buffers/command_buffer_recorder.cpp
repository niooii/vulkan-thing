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

        Utils::ExpectBadResult("Failed to allocate command buffers", vkAllocateCommandBuffers(device.vk_device(), &buffer_alloc_info, &vk_buffer));

        spdlog::debug("Vulkan cmd buffer recorder initialized.");
    }

    CommandBufferRecorder::~CommandBufferRecorder() {
        vkDestroyCommandPool(device_.vk_device(), vk_cmd_pool_, nullptr);
        spdlog::debug("Vulkan cmd buffer recorder destroyed.");
    }

    void CommandBufferRecorder::RecordCommandBuffers(uint32_t image_index) {
        VkRenderPassBeginInfo rp_info{};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.framebuffer = swapchain_.frame_buffers()[image_index];
        rp_info.renderPass = graphics_pipeline_.render_pass();



        vkCmdBeginRenderPass(vk_buffer, rp_info, );
    }

}