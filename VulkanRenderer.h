//#include <vector>
//#include <set>
//#include <vulkan/vulkan.hpp>
//
//#include <SDL.h>
//#include <SDL_vulkan.h>
//#include <SDL_video.h>
//// #include "Window.h"
//#include "Globals.h"
//#include "Utils.h"
//#include "vulkan/vulkan_handles.hpp"
//
//using namespace vk;
//
//class VulkanRenderer {
//public:
//  VulkanRenderer();
//  ~VulkanRenderer();
//
//  Result init(SDL_Window *windowTarget);
//
//private:
//  SDL_Window *sdl_window_;
//
//  // vulkan components
//  Instance instance_;
//  struct {
//    PhysicalDevice physical;
//    Device logical;
//  } mainDevice;
//
//  SurfaceKHR surface;
//  Queue graphicsQueue;
//  Queue presentationQueue;
//
//  SwapchainKHR swapchain;
//
//  // vulkan functions
//  void createInstance();
//  void initDevice();
//  void createSurface();
//  void createSwapchain();
//
//  // validation funcrions
//  // check if vk_device is suitable for use
//  bool validateDevice(PhysicalDevice vk_device);
//
//  bool instanceExtensionSupport(std::vector<const char*> *extensionNames);
//  // check if vk_device supports swap chain
//  bool deviceExtensionSupport(PhysicalDevice vk_device);
//
//  QueueFamilyIndices getQueueFamilies(PhysicalDevice vk_device);
//  SwapChainDetails getSwapChainDetails(PhysicalDevice vk_device);
//
//  // choose swapchain settings
//  SurfaceFormatKHR getOptimalSurfaceFormat(const vector<SurfaceFormatKHR>& formats);
//  PresentModeKHR getOptimalPresentationMode(const vector<vk::PresentModeKHR> &presentModes);
//  Extent2D getSwapExtent(const SurfaceCapabilitiesKHR& surfaceCapabilities);
//
//  // cleanup resources
//  void cleanup();
//};
