#include <vector>
#include <vulkan/vulkan.hpp>

#include <SDL.h>
#include <SDL_vulkan.h>
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