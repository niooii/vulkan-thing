#include "CommandBufferRecorder.h"

namespace Engine::Vulkan {

    CommandBufferRecorder::CommandBufferRecorder(Device& device, Swapchain& swapchain, GraphicsPipeline& graphics_pipeline)
    : device_(device), swapchain_(swapchain), graphics_pipeline_(graphics_pipeline){

    }

    CommandBufferRecorder::~CommandBufferRecorder() {
        vkDestroyCommandPool(device_.vk_device(), vk_cmd_pool_, nullptr);
    }

    void CommandBufferRecorder::RecordCommandBuffers() {

    }


}