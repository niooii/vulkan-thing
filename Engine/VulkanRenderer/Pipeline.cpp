//
// Created by niooi on 1/30/24.
//

#include "Pipeline.h"

namespace Engine::Vulkan {

    Pipeline::Pipeline(Device &device, Swapchain& swapchain) : device_(device), swapchain_(swapchain) {
        std::string_view vert_path{"../Shaders/triangle.vert.spv"};
        std::string_view frag_path{"../Shaders/triangle.frag.spv"};
        vert_shader_module_ = CreateShaderModule(vert_path);
        frag_shader_module_ = CreateShaderModule(frag_path);

        /*
         * Because we're hard coding the vertex data directly in the vertex shader,
         * we'll fill in this structure to specify that
         * there is no vertex data to load for now.
         * We'll get back to it in the vertex buffer chapter.
         * LOL
         */

        VkPipelineVertexInputStateCreateInfo vertex_input_info{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexBindingDescriptionCount = 0;
        vertex_input_info.pVertexBindingDescriptions = nullptr; // optional
        vertex_input_info.vertexAttributeDescriptionCount = 0;
        vertex_input_info.pVertexAttributeDescriptions = nullptr; // optional

        VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
        input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_info.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchain_.extent().width;
        viewport.height = (float) swapchain_.extent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain_.extent();

        std::vector<VkDynamicState> dynamic_states = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_info{};
        dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
        dynamic_state_info.pDynamicStates = dynamic_states.data();

        VkPipelineViewportStateCreateInfo viewport_info{};
        viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_info.pViewports = &viewport;
        viewport_info.viewportCount = 1;
        viewport_info.pScissors = &scissor;
        viewport_info.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer_info{};
        rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer_info.depthClampEnable = VK_FALSE;
        // geometry never passes through rasterization stage. no more frames
        rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
        rasterizer_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer_info.lineWidth = 1.0f;
        rasterizer_info.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;

        // alteration of depth values and stuff
        rasterizer_info.depthBiasEnable = VK_FALSE;
        rasterizer_info.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer_info.depthBiasClamp = 0.0f; // Optional
        rasterizer_info.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multisample_info{};
        multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_info.sampleShadingEnable = VK_FALSE;
        multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample_info.minSampleShading = 1.0f; // Optional
        multisample_info.pSampleMask = nullptr; // Optional
        multisample_info.alphaToCoverageEnable = VK_FALSE; // Optional
        multisample_info.alphaToOneEnable = VK_FALSE; // Optional

        // TODO! depth and stencil testing

        VkPipelineColorBlendAttachmentState blend_attachment_info{};
        blend_attachment_info.alphaBlendOp = VK_BLEND_OP_XOR_EXT;

        spdlog::debug("Pipeline initialized.");
    }

    void Pipeline::Destroy() {
        // TODO! replace with destroy all shader modules later
        vkDestroyShaderModule(device_.vk_device(), vert_shader_module_, nullptr);
        vkDestroyShaderModule(device_.vk_device(), frag_shader_module_, nullptr);

        vkDestroyPipeline(device_.vk_device(), vk_pipeline_, nullptr);
        vkDestroyPipelineLayout(device_.vk_device(), vk_pipeline_layout_, nullptr);
    }

    VkShaderModule Pipeline::CreateShaderModule(std::string_view& filename) {
        std::vector<char> code = Utils::ReadFile(filename);
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule module;

        VkResult result = vkCreateShaderModule(device_.vk_device(), &create_info, nullptr, &module);

        Utils::ExpectBadResult("Failed to create shader module", result);

        return module;
    }

}