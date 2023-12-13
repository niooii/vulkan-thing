#include <vector>
#include <vulkan/vulkan.hpp>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_video.h>
// #include "Window.h"
#include "Globals.h"

class VulkanRenderer {
public:
  VulkanRenderer();
  ~VulkanRenderer();

  vk::Result init(SDL_Window *windowTarget);

private:
  SDL_Window *window;

  // vulkan components
  vk::Instance instance;

  // vulkan functions
  void createInstance();
  bool extensionsAreSupported(std::vector<const char*>* extensionNames);
};