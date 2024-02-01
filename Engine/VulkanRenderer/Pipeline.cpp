//
// Created by niooi on 1/30/24.
//

#include "Pipeline.h"

namespace Engine::Vulkan {

    Pipeline::Pipeline(Device &device) : device_(device) {
        std::string_view vert_path{"Shaders/triangle.vert.spv"};
        std::string_view frag_path{"Shaders/triangle.frag.spv"};
        vert_shader_module_ = CreateShaderModule(vert_path);
        frag_shader_module_ = CreateShaderModule(frag_path);

        spdlog::debug("Pipeline initialized.");
    }

    void Pipeline::Destroy() {
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