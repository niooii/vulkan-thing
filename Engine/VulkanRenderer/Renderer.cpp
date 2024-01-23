#include "Renderer.h"

namespace VulkanRenderer {
    Renderer::Renderer(const char* application_name, const char* engine_name, SDL_Window* window_ptr)
    : instance(Instance(application_name, engine_name, window_ptr)) {

    }

    Renderer::~Renderer() {

    }
}