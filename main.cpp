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

const int W_WIDTH = 900;
const int W_HEIGHT = 600;

#undef main

void createInstance(SDL_Window* window) {
    
}

int main() {
    std::cout << "hi" << '\n';

    VulkanRenderer vkRenderer{};

    Window window{};

    window.create();

    vkRenderer.init(window.raw());

    // SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    // auto flags = SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI | SDL_WindowFlags::SDL_WINDOW_VULKAN | SDL_WindowFlags::SDL_WINDOW_RESIZABLE;
    
    // SDL_Window* window = SDL_CreateWindow("const char *title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, flags); 
    
    // createInstance(window);

    // // main loop
    // bool running{true};
    // SDL_Event event;
    
    // while(running) {
    //     while(SDL_PollEvent(&event)) {
    //         switch (event.type) {
    //             case SDL_QUIT:
    //                 running = false;
    //                 break;
                
    //         }
    //     }
    // }

    return 0;
}
