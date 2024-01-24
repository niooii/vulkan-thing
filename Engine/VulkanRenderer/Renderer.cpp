#include "Renderer.h"

namespace Engine::Vulkan {

    Renderer::Renderer(const char* application_name, const char* engine_name, SDL_Window* window_ptr, bool validation_layers_enabled)
    : instance_(Instance(application_name, engine_name, window_ptr, validation_layers_enabled)),
    device_(Device()),
    swapchain_(Swapchain(device_)) {

    }

    Renderer::~Renderer() {
        swapchain_.Destroy();
        device_.Destroy();
        instance_.Destroy();
    }

}