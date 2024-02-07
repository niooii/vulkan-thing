#ifndef VULKAN_STUFF_GRAPHICS_PIPELINE_H
#define VULKAN_STUFF_GRAPHICS_PIPELINE_H

#include "renderer/vulkan/device/device.h"
#include "renderer/vulkan/swapchain/swapchain.h"
#include "utils.h"

namespace Engine::Vulkan {

    class Swapchain;

    class GraphicsPipeline {
    public:
        GraphicsPipeline(Device& device, Swapchain& swapchain);
        ~GraphicsPipeline();

        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator= (const GraphicsPipeline&) = delete;

        // Accessors
        VkRenderPass render_pass();

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