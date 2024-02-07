#include "graphics_pipeline.h"

namespace Engine::Vulkan {

    GraphicsPipeline::GraphicsPipeline(Device& device, Swapchain& swapchain) : device_(device), swapchain_(swapchain) {
        std::string_view vert_path{"../Shaders/triangle.vert.spv"};
        std::string_view frag_path{"../Shaders/triangle.frag.spv"};
        vert_shader_module_ = CreateShaderModule(vert_path);
        frag_shader_module_ = CreateShaderModule(frag_path);

        VkPipelineShaderStageCreateInfo vertex_shader_info{};
        vertex_shader_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_info.module = vert_shader_module_;
        vertex_shader_info.pName = "main";

        VkPipelineShaderStageCreateInfo frag_shader_info{};
        frag_shader_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_shader_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_shader_info.module = frag_shader_module_;
        frag_shader_info.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertex_shader_info, frag_shader_info};

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

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable = VK_TRUE;
        color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_info{};
        color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_info.logicOpEnable = VK_FALSE;
        color_blend_info.logicOp = VK_LOGIC_OP_COPY; // Optional
        color_blend_info.attachmentCount = 1;
        color_blend_info.pAttachments = &color_blend_attachment;
        color_blend_info.blendConstants[0] = 0.0f; // Optional
        color_blend_info.blendConstants[1] = 0.0f; // Optional
        color_blend_info.blendConstants[2] = 0.0f; // Optional
        color_blend_info.blendConstants[3] = 0.0f; // Optional

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 0; // Optional
        pipeline_layout_info.pSetLayouts = nullptr; // Optional
        pipeline_layout_info.pushConstantRangeCount = 0; // Optional
        pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

        vkCreatePipelineLayout(device_.vk_device(), &pipeline_layout_info, nullptr, &vk_pipeline_layout_);

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = 2;
        pipeline_info.pStages = shaderStages;
        pipeline_info.pVertexInputState = &vertex_input_info;
        pipeline_info.pInputAssemblyState = &input_assembly_info;
        pipeline_info.pViewportState = &viewport_info;
        pipeline_info.pRasterizationState = &rasterizer_info;
        pipeline_info.pMultisampleState = &multisample_info;
        pipeline_info.pDepthStencilState = nullptr; // Optional
        pipeline_info.pColorBlendState = &color_blend_info;
        pipeline_info.pDynamicState = &dynamic_state_info;

        pipeline_info.layout = vk_pipeline_layout_;

        render_pass_ = CreateRenderPass();

        pipeline_info.renderPass = render_pass_;
        pipeline_info.subpass = 0;

        // weird optional stuff
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_info.basePipelineIndex= -1;

        vkCreateGraphicsPipelines(device_.vk_device(), nullptr, 1, &pipeline_info, nullptr, &vk_pipeline_);

        VkDebugUtilsMessengerCreateInfoEXT dumci{};

        spdlog::debug("Graphics Pipeline initialized.");
    }

    GraphicsPipeline::~GraphicsPipeline() {
        // TODO! replace with destroy all shader modules later
        vkDestroyShaderModule(device_.vk_device(), vert_shader_module_, nullptr);
        vkDestroyShaderModule(device_.vk_device(), frag_shader_module_, nullptr);

        vkDestroyRenderPass(device_.vk_device(), render_pass_, nullptr);

        vkDestroyPipeline(device_.vk_device(), vk_pipeline_, nullptr);
        vkDestroyPipelineLayout(device_.vk_device(), vk_pipeline_layout_, nullptr);
    }

    VkRenderPass GraphicsPipeline::render_pass() {
        return render_pass_;
    }

    VkShaderModule GraphicsPipeline::CreateShaderModule(std::string_view& filename) {
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

    VkRenderPass GraphicsPipeline::CreateRenderPass() {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = swapchain_.image_format();
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;

        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        // no stencil for now
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;

        VkRenderPass render_pass;

        VkResult result = vkCreateRenderPass(device_.vk_device(), &render_pass_info, nullptr, &render_pass);

        Utils::ExpectBadResult("Failed to create render pass.", result);

        return render_pass;
    }

}