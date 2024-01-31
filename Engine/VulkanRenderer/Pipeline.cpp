//
// Created by niooi on 1/30/24.
//

#include "Pipeline.h"

namespace Engine::Vulkan {

    Pipeline::Pipeline(Device &device) : device_(device) {

    }

    void Pipeline::Destroy() {
        vkDestroyPipeline(device_.vk_device(), vk_pipeline_, nullptr);
        vkDestroyPipelineLayout(device_.vk_device(), vk_pipeline_layout_, nullptr);
    }

    VkShaderModule Pipeline::CreateShaderModule(std::string_view& filename) {
        std::vector<char> code = Utils::ReadFile(filename);
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        todo!()
    }

}