#include <vector>
#include <vulkan/vulkan.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL_video.h>
// #include "Window.h"

class VulkanRenderer {
public:
  VulkanRenderer();

  int init(SDL_Window *windowTarget);

private:
  SDL_Window *window;

  // vulkan components
};