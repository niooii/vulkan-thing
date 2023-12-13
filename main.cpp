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
#include "Window.h"
#include "Globals.h"

const int W_WIDTH = 900;
const int W_HEIGHT = 600;

#undef main

void createInstance(SDL_Window* window) {
    
}

int main() {
    std::cout << "hi" << '\n';

    globals::logger.activateDebug();
    globals::logger.activateErr();

    VulkanRenderer vkRenderer{};

    Window window{};

    window.create();

    vkRenderer.init(window.raw());

    return 0;
}
