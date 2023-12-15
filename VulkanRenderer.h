#include <vector>
#include <vulkan/vulkan.hpp>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_video.h>
// #include "Window.h"
#include "Globals.h"
#include "Utils.h"
#include "vulkan/vulkan_handles.hpp"

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
  struct {
    PhysicalDevice physical;
    Device logical;
  } mainDevice;

  // vulkan functions
  void createInstance();

  void initDevice();
  
  // validation funcrions
  // check if device is suitable for use
  bool validateDevice(PhysicalDevice& device);
  
  bool extensionsAreSupported(std::vector<const char*>* extensionNames);

  QueueFamilyIndices getQueueFamilies(PhysicalDevice device);
};
