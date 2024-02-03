#ifndef VULKAN_STUFF_GRAPHICSPIPELINE_H
#define VULKAN_STUFF_GRAPHICSPIPELINE_H

#include "VulkanRenderer/Device/Device.h"
#include "Utils.h"
#include "Swapchain.h"

namespace Engine::Vulkan {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(Device& device, Swapchain& swapchain);
        void Destroy();

    private:
        Device& device_;
        Swapchain& swapchain_;

        VkPipeline vk_pipeline_;
        VkPipelineLayout vk_pipeline_layout_;
        VkRenderPass render_pass_;
        VkShaderModule vert_shader_module_;
        VkShaderModule frag_shader_module_;

        // Internal
        VkShaderModule CreateShaderModule(std::string_view& filename);
        VkRenderPass CreateRenderPass();
    };

}

#endif