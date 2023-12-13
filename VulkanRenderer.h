#include <vector>
#include <vulkan/vulkan.hpp>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_video.h>
// #include "Window.h"
#include "Globals.h"

using namespace vk;

class VulkanRenderer {
public:
  VulkanRenderer();
  ~VulkanRenderer();

  Result init(SDL_Window *windowTarget);

private:
  SDL_Window *window;

  // vulkan components
  Instance instance;
  struct MainDevice {
    PhysicalDevice physical;
    Device logical;
  };

  // vulkan functions
  void createInstance();

  void getPhysicalDevice();

  bool extensionsAreSupported(std::vector<const char*>* extensionNames);
};
