#ifndef VULKAN_STUFF_VK_RENDERER_H
#define VULKAN_STUFF_VK_RENDERER_H

#include "instance.h"
#include "renderer/vulkan/swapchain/swapchain.h"
#include "surface.h"
#include "graphics_pipeline.h"
#include "device/device.h"
#include "device/device_utils.h"
#include "utils.h"
#include "core/clock.h"
#include <renderer/renderer_interface.h>
#include <renderer/vulkan/buffers/command_buffer_recorder.h>

#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace Engine::Vulkan {

    class VkRenderer : public RendererInterface {
    public:
        // The parameters passed in are used to create the underlying Vulkan opt_instance_.
        VkRenderer(const char* application_name, Window &window, bool debug);
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

        void DrawFrame() override;

    private:
        std::optional<Instance> opt_instance_;
        std::optional<Surface> opt_surface_;
        std::optional<Device> opt_device_;
        std::optional<Swapchain> opt_swapchain_;
        std::optional<GraphicsPipeline> opt_graphics_pipeline_;
        std::optional<CommandBufferRecorder> opt_cmd_recorder_;

        // stats && analytics
        Core::Clock clock_{};

        // workaround late initialization
        Instance* instance_;
        Surface* surface_;
        Device* device_;
        Swapchain* swapchain_;
        GraphicsPipeline* graphics_pipeline_;
        CommandBufferRecorder* cmd_recorder_;
    };

}

#endif
