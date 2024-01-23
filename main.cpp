#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_events.h>
#include <SDL_hints.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_to_string.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include "VulkanRenderer.h"
#include "Engine/Window.h"
#include "Globals.h"

#include "Engine/VulkanRenderer/Renderer.h"

const int W_WIDTH = 900;
const int W_HEIGHT = 600;

#undef main

int main() {
    std::cout << "PRECOMPILED MOMENT" << '\n';

    globals::logger.activateDebug();
    globals::logger.activateErr();

//    VulkanRenderer vkRenderer{};



    Window window{
        "A test window....",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        800
    };

    VulkanRenderer::Renderer renderer{
        "app name",
        "engine name",
        window.Raw()
    };

    while(!window.RequestedQuit()) {
        window.PollEvents();
    }

    return 0;
}
