#include "vk_renderer.h"

namespace Engine::Vulkan {

    VkRenderer::VkRenderer(const char* application_name, Window& window, bool validation_layers_enabled) {

        // init instance
        instance_.emplace(application_name, "onion engine", window, validation_layers_enabled);
        Instance& instance = instance_.value();

        surface_.emplace(
                instance, window
                );
        Surface& surface = surface_.value();

        std::vector<PhysicalDevice> physical_devices = DeviceUtils::GetPhysicalDevices(instance);

        assert(!physical_devices.empty());
        spdlog::debug("Using device {}.", physical_devices[0].name());
        device_.emplace(
                instance, surface, physical_devices[0]
                );
        Device& device = device_.value();

        swapchain_.emplace(
                device, instance, surface, window
                );
        Swapchain& swapchain = swapchain_.value();

        graphics_pipeline_.emplace(
                /// may havae to remove
                device, swapchain
                );
        GraphicsPipeline& graphics_pipeline = graphics_pipeline_.value();
        swapchain.CreateFramebuffers(graphics_pipeline.render_pass());

        cmd_recorder_.emplace(
                device, swapchain, graphics_pipeline
                );
        CommandBufferRecorder& cmd_recorder = cmd_recorder_.value();

        spdlog::info("Finished renderer initialization.");
    }

    VkRenderer::~VkRenderer() {
        spdlog::info("Shutting down renderer...");
    }

    void VkRenderer::Resize(u16 w, u16 h) {

    }

    bool VkRenderer::BeginFrame(f32 delta_time) {
        return true;
    }

    bool VkRenderer::EndFrame(f32 delta_time) {
        return true;
    }

}