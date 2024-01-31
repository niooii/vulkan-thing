#ifndef VULKAN_STUFF_PIPELINE_H
#define VULKAN_STUFF_PIPELINE_H

#include "Device.h"
#include "Utils.h"

namespace Engine::Vulkan {

    class Pipeline {
    public:
        Pipeline(Device& device);
        void Destroy();

    private:
        Device& device_;

        VkPipeline vk_pipeline_;
        VkPipelineLayout vk_pipeline_layout_;
        VkShaderModule vert_shader_module_;
        VkShaderModule frag_shader_module_;

        // Internal
        VkShaderModule CreateShaderModule(std::string_view& filename);
    };

}

#endif