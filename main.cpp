#include "engine/window.h"
#include "engine/renderer/vulkan/vk_renderer.h"
#include "engine/renderer/renderer.h"

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

    Engine::Renderer renderer{window,  Engine::BackendType::VULKAN, "hi world LOL"};

    while(!window.requested_quit()) {
        window.PollEvents();
        renderer.DrawFrame();
    }
    return 0;
}
