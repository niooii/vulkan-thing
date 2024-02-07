#ifndef VULKAN_STUFF_VK_RENDERER_H
#define VULKAN_STUFF_VK_RENDERER_H

#include "instance.h"
#include "renderer/vulkan/swapchain/swapchain.h"
#include "surface.h"
#include "graphics_pipeline.h"
#include "device/device.h"
#include "device/device_utils.h"
#include "utils.h"
#include <renderer/renderer_interface.h>
#include <renderer/vulkan/buffers/CommandBufferRecorder.h>

#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace Engine::Vulkan {

    class VkRenderer : public RendererInterface {
    public:
        // The parameters passed in are used to create the underlying Vulkan instance_.
        VkRenderer(const char* application_name, Window &window, bool validation_layers_enabled);
        // Destroys all resources in *deliberate* order.
        ~VkRenderer() override;

        VkRenderer(const VkRenderer&) = delete;
        VkRenderer& operator= (VkRenderer&) = delete;

        // override rendering backend

        void Resize(u16 w, u16 h) override;

        // If successful, returns true
        bool BeginFrame(f32 delta_time) override;

        // If successful, return true
        bool EndFrame(f32 delta_time) override;

    private:
        std::optional<Instance> instance_;
        std::optional<Surface> surface_;
        std::optional<Device> device_;
        std::optional<Swapchain> swapchain_;
        std::optional<GraphicsPipeline> graphics_pipeline_;
        std::optional<CommandBufferRecorder> cmd_recorder_;
    };

}

#endif
