#include "Engine/Window.h"
#include "Engine/VulkanRenderer/Renderer.h"

#include <spdlog/spdlog.h>

const int W_WIDTH = 900;
const int W_HEIGHT = 600;

#undef main

int main() {

    spdlog::set_level(spdlog::level::debug);

    Engine::Window window{
        "A test window....",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        W_WIDTH,
        W_HEIGHT
    };

    spdlog::debug("created window heheeaw");

    bool validation_layers_enabled = true;
    Engine::Vulkan::Renderer renderer{
        "app name",
        "engine name",
        window,
        validation_layers_enabled
    };

    while(!window.requested_quit()) {
        window.PollEvents();
    }
    return 0;
}
