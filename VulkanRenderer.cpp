//#include "VulkanRenderer.h"
//#include "Utils.h"
//#include "vulkan/vulkan.hpp"
//#include "vulkan/vulkan_enums.hpp"
//#include "vulkan/vulkan_handles.hpp"
//#include "vulkan/vulkan_structs.hpp"
//#include "vulkan/vulkan_to_string.hpp"
//#include <SDL_video.h>
//#include <cstdint>
//#include <limits>
//#include <stdexcept>
//#include <vector>
//#include <algorithm>
//
//using std::vector;
//using std::set;
//using globals::logger;
//
//using namespace vk;
//
//VulkanRenderer::VulkanRenderer() = default;
//VulkanRenderer::~VulkanRenderer() {
//    cleanup();
//};
//
//Result VulkanRenderer::init(SDL_Window* windowTarget) {
//
//    sdl_window_ = windowTarget;
//
//    createInstance();
//    createSurface();
//    initDevice();
//    createSwapchain();
//
//
//    return Result::eSuccess;
//}
//
//void VulkanRenderer::createInstance() {
//
//    ApplicationInfo appInfo{};
//    appInfo.sType = vk::StructureType::eApplicationInfo;
//    appInfo.pApplicationName = "Vulkan app woo";
//    appInfo.applicationVersion = vk::makeApiVersion(1, 0, 0, 0);
//    appInfo.pEngineName = "No Engine";
//    appInfo.engineVersion = vk::makeApiVersion(1, 0, 0, 0);
//    appInfo.apiVersion = vk::ApiVersion13;
//
//    InstanceCreateInfo createInfo{};
//    createInfo.sType = StructureType::eInstanceCreateInfo;
//    createInfo.pApplicationInfo = &appInfo;
//
//    // grab extensions from sdl
//    unsigned int extensionCount;
//    SDL_Vulkan_GetInstanceExtensions(sdl_window_, &extensionCount, nullptr);
//    vector<const char*> extensionNames{extensionCount};
//    SDL_Vulkan_GetInstanceExtensions(sdl_window_, &extensionCount, extensionNames.data());
//
//    // check available extensions
//    instanceExtensionSupport(&extensionNames);
//
//    createInfo.ppEnabledExtensionNames = extensionNames.data();
//    createInfo.enabledExtensionCount = extensionNames.size();
//
////    for(const char*& name : extensionNames) {
////        logger.debug(name);
////    }
//
//    Result result;
//    if ((result = ::createInstance(&createInfo, nullptr, &instance_)) != Result::eSuccess) {
//        logger.err("Failed to create vulkan instance_. Error: ");
//        logger.err(vk::to_string(result).c_str());
//        throw std::runtime_error("failed to create instance_!");
//    }
//
//    logger.debug("Vulkan instance_ successfully initialized.");
//
//    // testing some vk_device stuff
//    auto devices = instance_.enumeratePhysicalDevices();
//
//    for(auto& vk_device : devices) {
//        logger.debug("there is a vk_device. ");
//    }
//
//    if(devices.empty()) {
//        logger.warn("There is no vk_device available.");
//    }
//}
//
//void VulkanRenderer::initDevice() {
//
//    unsigned int count;
//    instance_.enumeratePhysicalDevices(&count, nullptr);
//
//    if(count == 0) {
//        logger.err("No vulkan supported devices found.");
//        throw std::runtime_error("failed to find vk_device.");
//    }
//
//    vector<PhysicalDevice> devices{count};
//    instance_.enumeratePhysicalDevices(&count, devices.data());
//    bool deviceFound{false};
//    // find first suitable vk_device
//    for(auto& vk_device : devices) {
//        // logger.debug(std::format("Found device: {}", vk_device.getProperties().deviceName));
//        if(validateDevice(vk_device)) {
//            mainDevice.physical = vk_device;
//            deviceFound = true;
//            break;
//        }
//    }
//
//    if(!deviceFound) {
//        logger.err("No suitable vk_device found.");
//        throw std::runtime_error("GG");
//    }
//
//    if(devices.size() == 0) {
//        logger.err("No vulkan-supported vk_device found.");
//        throw std::runtime_error("GG");
//    }
//
//    // create queues
//    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physical);
//
//    vector<DeviceQueueCreateInfo> qCreateInfos;
//    set<int> qFamilyIndices {indices.graphicsFamily, indices.presentationFamily};
//
//    // all info for the queues needed
//    for(int qIdx : qFamilyIndices) {
//        DeviceQueueCreateInfo qCreateInfo{};
//        qCreateInfo.sType = StructureType::eDeviceQueueCreateInfo;
//        qCreateInfo.queueFamilyIndex = qIdx;
//        float priority = 1.0f;
//        qCreateInfo.pQueuePriorities = &priority;
//        // just 1 for now
//        qCreateInfo.queueCount = 1;
//
//        qCreateInfos.push_back(qCreateInfo);
//    }
//
//
//    // create queues && then create logical vk_device
//    DeviceCreateInfo dCreateInfo{};
//    dCreateInfo.sType = StructureType::eDeviceCreateInfo;
//
//    // a list of queues to create.
//    dCreateInfo.queueCreateInfoCount = qCreateInfos.size();
//    dCreateInfo.pQueueCreateInfos = qCreateInfos.data();
//
//    dCreateInfo.enabledExtensionCount = deviceExtensions.size();
//    dCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
//
//    // physhical vk_device features logical will use
//    PhysicalDeviceFeatures pFeatures{};
//
//    dCreateInfo.pEnabledFeatures = &pFeatures;
//
//    // create the vk_device
//    Result res = mainDevice.physical.createDevice(&dCreateInfo, nullptr, &mainDevice.logical);
//
//    if(res != Result::eSuccess) {
//        logger.err("failed to create logical vk_device");
//        logger.err(vk::to_string(res).c_str());
//        throw std::runtime_error("NO DEVICE FOR YOU");
//    }
//
//    // get handle to queue(s)
//    mainDevice.logical.getQueue(indices.graphicsFamily, 0, &graphicsQueue);
//    mainDevice.logical.getQueue(indices.presentationFamily, 0, &presentationQueue);
//
//}
//
//void VulkanRenderer::createSurface() {
//    VkSurfaceKHR surfaceKhr;
//
//    SDL_bool res = SDL_Vulkan_CreateSurface(sdl_window_, instance_, &surfaceKhr);
//
//    if(res == SDL_FALSE) {
//        throw std::runtime_error("SURFACE FAILED");
//    }
//
//    surface = SurfaceKHR(surfaceKhr);
//}
//
//bool VulkanRenderer::validateDevice(PhysicalDevice vk_device) {
//    PhysicalDeviceProperties properties = vk_device.getProperties();
//    PhysicalDeviceFeatures features = vk_device.getFeatures();
//
//    QueueFamilyIndices indices = getQueueFamilies(vk_device);
//    bool extensionsSupported = deviceExtensionSupport(vk_device);
//
//    SwapChainDetails swapChainDetails = getSwapChainDetails(vk_device);
//    bool swapChainValid{!swapChainDetails.formats.empty() && !swapChainDetails.presentationModes.empty()};
//
//    return indices.isValid() && extensionsSupported && swapChainValid;
//}
//
//bool VulkanRenderer::instanceExtensionSupport(std::vector<const char *> *extensionNames) {
//    bool allSupported = true;
//
//    // grab supported extension properties.
//    unsigned int count;
//    Result res = vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr);
//    vector<ExtensionProperties> extensionProperties{count};
//    res = vk::enumerateInstanceExtensionProperties(nullptr, &count, extensionProperties.data());
//
//    for(const char*& name : *extensionNames) {
//        bool found = false;
//        for(const ExtensionProperties& ext : extensionProperties) {
//            if(strcmp(ext.extensionName, name)) {
//                found = true;
//                break;
//            }
//        }
//
//        if(!found) {
//            logger.err(std::format("Vulkan extension {} is not supported.", name));
//            allSupported = false;
//        }
//    }
//
//    return allSupported;
//}
//
//bool VulkanRenderer::deviceExtensionSupport(PhysicalDevice vk_device) {
//    unsigned int extensionCount;
//
//    vk_device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//    if(extensionCount == 0) {
//        return false;
//    }
//
//    vector<ExtensionProperties> extensions{extensionCount};
//    vk_device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, extensions.data());
//
//    // check if the enumerated extensions contains the extensions
//    // listed in the Utils.h deviceExtensions vec.
//    for(const auto& deviceExtension : deviceExtensions) {
//        bool hasExtension{false};
//
//        for(const auto& extension : extensions) {
//            if(strcmp(deviceExtension, extension.extensionName) == 0) {
//                hasExtension = true;
//                break;
//            }
//        }
//
//        // if one of these arent supported, then we wanna break immediately.
//        if(!hasExtension) {
//            return false;
//        }
//
//    }
//
//    return true;
//}
//
//QueueFamilyIndices VulkanRenderer::getQueueFamilies(PhysicalDevice vk_device) {
//    QueueFamilyIndices indices{};
//
//    unsigned int qFamilyCount;
//    vk_device.getQueueFamilyProperties(&qFamilyCount, nullptr);
//    vector<QueueFamilyProperties> qFamilyProperties{qFamilyCount};
//    vk_device.getQueueFamilyProperties(&qFamilyCount, qFamilyProperties.data());
//
//    int i = 0;
//    for(const auto& family : qFamilyProperties) {
//
//        // check if its a graphics queue and theres more than 0 queues
//        if(family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics) {
//            indices.graphicsFamily = i;
//        }
//
//        // check if the q family supports presentation
//        Bool32 presentationSupport = false;
//        vk_device.getSurfaceSupportKHR(i, surface, &presentationSupport);
//        if(family.queueCount > 0 && presentationSupport) {
//            indices.presentationFamily = i;
//        }
//
//        if(indices.isValid()) {
//            break;
//        }
//
//        i++;
//    }
//
//    return indices;
//}
//
//void VulkanRenderer::createSwapchain() {
//    SwapChainDetails scDetails = getSwapChainDetails(mainDevice.physical);
//
//    SurfaceFormatKHR format = getOptimalSurfaceFormat(scDetails.formats);
//    PresentModeKHR presentMode = getOptimalPresentationMode(scDetails.presentationModes);
//    Extent2D extent = getSwapExtent(scDetails.surfaceCapabilities);
//
//    // how many images in the swap chain. set +1 than minimum for triple buffering
//    uint32_t imageCount = scDetails.surfaceCapabilities.minImageCount + 1;
//
//    // if imagecount is 0 then its unlimited
//    // also clamp down to max
//    if(imageCount > scDetails.surfaceCapabilities.maxImageCount && imageCount != 0) {
//        imageCount = scDetails.surfaceCapabilities.maxImageCount;
//    }
//
//    SwapchainCreateInfoKHR createInfo{};
//
//    createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
//    createInfo.presentMode = presentMode;
//    createInfo.imageExtent = extent;
//    createInfo.imageFormat = format.format;
//    createInfo.imageColorSpace = format.colorSpace;
//    createInfo.minImageCount = imageCount;
//    createInfo.imageArrayLayers = 1;
//    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
//    createInfo.preTransform = scDetails.surfaceCapabilities.currentTransform;
//    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
//    createInfo.clipped = vk::True;
//
//    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physical);
//
//    // if graphics and presentation families are different, then swapchain m sut be shared between families
//    if(indices.graphicsFamily != indices.presentationFamily) {
//        unsigned int qindices[] = {
//                (unsigned int)indices.graphicsFamily,
//                (unsigned int)indices.presentationFamily
//        };
//
//        createInfo.pQueueFamilyIndices = qindices;
//    } else {
//        createInfo.pQueueFamilyIndices =
//    }
//
////    createInfo.oldSwapchain = vk::SwapchainKHR
//
//    SwapchainKHR swapchain{};
//
//    Result res = mainDevice.logical.createSwapchainKHR(&createInfo, nullptr, &swapchain);
//
//    if(res != Result::eSuccess) {
//        logger.err("Failed to create swapchain: ");
//        logger.err(vk::to_string(res));
//        throw std::runtime_error("GG");
//    }
//
//    this->swapchain = swapchain;
//
//    logger.debug("Create swapchain");
//}
//
//SwapChainDetails VulkanRenderer::getSwapChainDetails(PhysicalDevice vk_device) {
//
//    SwapChainDetails details{};
//    // -- GRAB CAPABILITIES --
//    vk_device.getSurfaceCapabilitiesKHR(surface, &details.surfaceCapabilities);
//
//    // -- GRAB FORMATS --
//    unsigned int formatCount;
//    vk_device.getSurfaceFormatsKHR(surface, &formatCount, nullptr);
//    details.formats.resize(formatCount);
//    vk_device.getSurfaceFormatsKHR(surface, &formatCount, details.formats.data());
//
//    // -- GRAB PRESENTATION MODES --
//    unsigned int pmodeCount;
//    vk_device.getSurfacePresentModesKHR(surface, &pmodeCount, nullptr);
//    details.presentationModes.resize(pmodeCount);
//    vk_device.getSurfacePresentModesKHR(surface, &pmodeCount, details.presentationModes.data());
//
//    return details;
//}
//
//SurfaceFormatKHR VulkanRenderer::getOptimalSurfaceFormat(const vector<SurfaceFormatKHR> &formats) {
//    // this means all formats are supported.
//    if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined) {
//        // normally the best formats to use
//        return {vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
//    }
//
//    for (const auto &format : formats) {
//        // find the ones that are regarded as optimal
//        if((format.format == vk::Format::eR8G8B8A8Unorm || format.format == vk::Format::eB8G8R8A8Unorm)
//        && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
//            return format;
//        }
//    }
//
//    // pray it works
//    return formats[0];
//}
//
//PresentModeKHR VulkanRenderer::getOptimalPresentationMode(const vector<vk::PresentModeKHR> &presentModes) {
//
//    for (const auto &mode : presentModes) {
//        // find the ones that are regarded as optimal
//        if(mode == vk::PresentModeKHR::eMailbox) {
//            return mode;
//        }
//    }
//
//    // should be available on every system.
//    return vk::PresentModeKHR::eFifo;
//}
//
//Extent2D VulkanRenderer::getSwapExtent(const SurfaceCapabilitiesKHR& surfaceCapabilities) {
//    Extent2D currentExtent = surfaceCapabilities.currentExtent;
//
//    // vulkan docs says if its equal to the max of uint32, it's of variable size.
//    if(currentExtent.width != std::numeric_limits<uint32_t>::max()) {
//        return currentExtent;
//    }
//
//    int w, h;
//    SDL_GetWindowSize(sdl_window_, &w, &h);
//    Extent2D extent = Extent2D{static_cast<uint32_t>(w), static_cast<uint32_t>(h)};
//
//    // clamp based on max and min value
//    extent.width = std::min(extent.width, surfaceCapabilities.maxImageExtent.width);
//    extent.height = std::min(extent.height, surfaceCapabilities.maxImageExtent.height);
//
//    extent.width = std::max(extent.width, surfaceCapabilities.minImageExtent.width);
//    extent.height = std::max(extent.height, surfaceCapabilities.minImageExtent.height);
//
//    return extent;
//
//}
//
//void VulkanRenderer::cleanup() {
//    vkDestroySurfaceKHR(instance_, surface, nullptr);
//    vkDestroyDevice(mainDevice.logical, nullptr);
//
//    vkDestroyInstance(instance_, nullptr);
//}
//
//
//
//
//
