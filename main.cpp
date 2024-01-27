#include "Engine/Window.h"
#include "Engine/VulkanRenderer/Renderer.h"

#include <iostream>

const int W_WIDTH = 900;
const int W_HEIGHT = 600;

#undef main

int main() {

    Engine::Window window{
        "A test window....",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        800
    };

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
