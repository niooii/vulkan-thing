#include <vector>
#include <set>
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

  SurfaceKHR surface;
  Queue graphicsQueue;
  Queue presentationQueue;

  // vulkan functions
  void createInstance();
  void initDevice();
  void createSurface();
  void createSwapchain();
  
  // validation funcrions
  // check if device is suitable for use
  bool validateDevice(PhysicalDevice device);
  
  bool instanceExtensionSupport(std::vector<const char*> *extensionNames);
  // check if device supports swap chain
  bool deviceExtensionSupport(PhysicalDevice device);

  QueueFamilyIndices getQueueFamilies(PhysicalDevice device);
  SwapChainDetails getSwapChainDetails(PhysicalDevice device);

  // choose swapchain settings
  SurfaceFormatKHR getOptimalSurfaceFormat(const vector<SurfaceFormatKHR>& formats);
  PresentModeKHR getOptimalPresentationMode(const vector<vk::PresentModeKHR> &presentModes);
  Extent2D getSwapExtent(const SurfaceCapabilitiesKHR& surfaceCapabilities);

  // cleanup resources
  void cleanup();
};
