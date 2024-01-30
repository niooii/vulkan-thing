#include "Renderer.h"

namespace Engine::Vulkan {

    Renderer::Renderer(const char* application_name, const char* engine_name, Window& window, bool validation_layers_enabled)
    : instance_(Instance(application_name, engine_name, window, validation_layers_enabled)),
    surface_(Surface(instance_, window)),
    device_(Device(instance_, surface_)),
    swapchain_(Swapchain(device_, instance_, surface_, window)) {
        spdlog::info("Finished renderer initialization.");
    }

    Renderer::~Renderer() {
        swapchain_.Destroy();
        device_.Destroy();
        surface_.Destroy();
        instance_.Destroy();
    }

}