#include "VulkanRenderer.h"
#include "Utils.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <SDL_video.h>
#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>

using std::vector;
using std::set;
using globals::logger;

using namespace vk;

VulkanRenderer::VulkanRenderer() = default;
VulkanRenderer::~VulkanRenderer() {
    cleanup();
};

Result VulkanRenderer::init(SDL_Window* windowTarget) {

    window = windowTarget;

    createInstance();
    createSurface();
    initDevice();

    
    return Result::eSuccess;
}

void VulkanRenderer::createInstance() {

    ApplicationInfo appInfo{};
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "Vulkan app woo";
    appInfo.applicationVersion = vk::makeApiVersion(1, 0, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = vk::makeApiVersion(1, 0, 0, 0);
    appInfo.apiVersion = vk::ApiVersion13;

    InstanceCreateInfo createInfo{};
    createInfo.sType = StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;

    // grab extensions from sdl
    unsigned int extensionCount;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    vector<const char*> extensionNames{extensionCount};
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

    // check available extensions
    instanceExtensionSupport(&extensionNames);

    createInfo.ppEnabledExtensionNames = extensionNames.data();
    createInfo.enabledExtensionCount = extensionNames.size();

//    for(const char*& name : extensionNames) {
//        logger.debug(name);
//    }

    Result result;
    if ((result = ::createInstance(&createInfo, nullptr, &instance)) != Result::eSuccess) {
        logger.err("Failed to create vulkan instance. Error: ");
        logger.err(vk::to_string(result).c_str());
        throw std::runtime_error("failed to create instance!");
    }

    logger.debug("Vulkan instance successfully initialized.");

    // testing some device stuff
    auto devices = instance.enumeratePhysicalDevices();

    for(auto& device : devices) {
        logger.debug("there is a device. ");
    }

    if(devices.empty()) {
        logger.warn("There is no device available.");
    }
}

void VulkanRenderer::initDevice() {

    unsigned int count;
    instance.enumeratePhysicalDevices(&count, nullptr);

    if(count == 0) {
        logger.err("No vulkan supported devices found.");
        throw std::runtime_error("failed to find device.");
    }

    vector<PhysicalDevice> devices{count};
    instance.enumeratePhysicalDevices(&count, devices.data());
    bool deviceFound{false};
    // find first suitable device
    for(auto& device : devices) {
        // logger.debug(std::format("Found device: {}", device.getProperties().deviceName));
        if(validateDevice(device)) {
            mainDevice.physical = device;
            deviceFound = true;
            break;
        }
    }

    if(!deviceFound) {
        logger.err("No suitable device found.");
        throw std::runtime_error("GG");
    }

    if(devices.size() == 0) {
        logger.err("No vulkan-supported device found.");
        throw std::runtime_error("GG");
    }

    // create queues
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physical);

    vector<DeviceQueueCreateInfo> qCreateInfos;
    set<int> qFamilyIndices {indices.graphicsFamily, indices.presentationFamily};

    // all info for the queues needed
    for(int qIdx : qFamilyIndices) {
        DeviceQueueCreateInfo qCreateInfo{};
        qCreateInfo.sType = StructureType::eDeviceQueueCreateInfo;
        qCreateInfo.queueFamilyIndex = qIdx;
        float priority = 1.0f;
        qCreateInfo.pQueuePriorities = &priority;
        // just 1 for now
        qCreateInfo.queueCount = 1;

        qCreateInfos.push_back(qCreateInfo);
    }


    // create queues && then create logical device
    DeviceCreateInfo dCreateInfo{};
    dCreateInfo.sType = StructureType::eDeviceCreateInfo; 

    // a list of queues to create.
    dCreateInfo.queueCreateInfoCount = qCreateInfos.size();
    dCreateInfo.pQueueCreateInfos = qCreateInfos.data();

    dCreateInfo.enabledExtensionCount = deviceExtensions.size();
    dCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // physhical device features logical will use
    PhysicalDeviceFeatures pFeatures{};

    dCreateInfo.pEnabledFeatures = &pFeatures;

    // create the device
    Result res = mainDevice.physical.createDevice(&dCreateInfo, nullptr, &mainDevice.logical);

    if(res != Result::eSuccess) {
        logger.err("failed to create logical device");
        logger.err(vk::to_string(res).c_str());
        throw std::runtime_error("NO DEVICE FOR YOU");
    }

    // get handle to queue(s)
    mainDevice.logical.getQueue(indices.graphicsFamily, 0, &graphicsQueue);
    mainDevice.logical.getQueue(indices.presentationFamily, 0, &presentationQueue);

}

void VulkanRenderer::createSurface() {
    VkSurfaceKHR surfaceKhr;

    SDL_bool res = SDL_Vulkan_CreateSurface(window, instance, &surfaceKhr);

    if(res == SDL_FALSE) {
        throw std::runtime_error("SURFACE FAILED");
    }

    surface = SurfaceKHR(surfaceKhr);
}

bool VulkanRenderer::validateDevice(PhysicalDevice device) {
    PhysicalDeviceProperties properties = device.getProperties();
    PhysicalDeviceFeatures features = device.getFeatures();

    QueueFamilyIndices indices = getQueueFamilies(device);
    bool extensionsSupported = deviceExtensionSupport(device);

    SwapChainDetails swapChainDetails = getSwapChainDetails(device);
    bool swapChainValid{!swapChainDetails.formats.empty() && !swapChainDetails.presentationModes.empty()};

    return indices.isValid() && extensionsSupported && swapChainValid;
}

bool VulkanRenderer::instanceExtensionSupport(std::vector<const char *> *extensionNames) {
    bool allSupported = true;

    // grab supported extension properties.
    unsigned int count;
    Result res = vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    vector<ExtensionProperties> extensionProperties{count};
    res = vk::enumerateInstanceExtensionProperties(nullptr, &count, extensionProperties.data());

    for(const char*& name : *extensionNames) {
        bool found = false;
        for(const ExtensionProperties& ext : extensionProperties) {
            if(strcmp(ext.extensionName, name)) {
                found = true;
                break;
            }
        }

        if(!found) {
            logger.err(std::format("Vulkan extension {} is not supported.", name));
            allSupported = false;
        }
    }

    return allSupported;
}

bool VulkanRenderer::deviceExtensionSupport(PhysicalDevice device) {
    unsigned int extensionCount;

    device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);

    if(extensionCount == 0) {
        return false;
    }

    vector<ExtensionProperties> extensions{extensionCount};
    device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, extensions.data());

    // check if the enumerated extensions contains the extensions
    // listed in the Utils.h deviceExtensions vec.
    for(const auto& deviceExtension : deviceExtensions) {
        bool hasExtension{false};

        for(const auto& extension : extensions) {
            if(strcmp(deviceExtension, extension.extensionName) == 0) {
                hasExtension = true;
                break;
            }
        }

        // if one of these arent supported, then we wanna break immediately.
        if(!hasExtension) {
            return false;
        }

    }

    return true;
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(PhysicalDevice device) {
    QueueFamilyIndices indices{};

    unsigned int qFamilyCount;
    device.getQueueFamilyProperties(&qFamilyCount, nullptr);
    vector<QueueFamilyProperties> qFamilyProperties{qFamilyCount};
    device.getQueueFamilyProperties(&qFamilyCount, qFamilyProperties.data());
    
    int i = 0;
    for(const auto& family : qFamilyProperties) {
        
        // check if its a graphics queue and theres more than 0 queues
        if(family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        // check if the q family supports presentation
        Bool32 presentationSupport = false;
        device.getSurfaceSupportKHR(i, surface, &presentationSupport);
        if(family.queueCount > 0 && presentationSupport) {
            indices.presentationFamily = i;
        }

        if(indices.isValid()) {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanRenderer::createSwapchain() {
    SwapChainDetails scDetails = getSwapChainDetails(mainDevice.physical);

    SurfaceFormatKHR format = getOptimalSurfaceFormat(scDetails.formats);
    PresentModeKHR presentMode = getOptimalPresentationMode(scDetails.presentationModes);
    Extent2D extent = getSwapExtent(scDetails.surfaceCapabilities);
}

SwapChainDetails VulkanRenderer::getSwapChainDetails(PhysicalDevice device) {

    SwapChainDetails details{};
    // -- GRAB CAPABILITIES --
    device.getSurfaceCapabilitiesKHR(surface, &details.surfaceCapabilities);

    // -- GRAB FORMATS --
    unsigned int formatCount;
    device.getSurfaceFormatsKHR(surface, &formatCount, nullptr);
    details.formats.resize(formatCount);
    device.getSurfaceFormatsKHR(surface, &formatCount, details.formats.data());

    // -- GRAB PRESENTATION MODES --
    unsigned int pmodeCount;
    device.getSurfacePresentModesKHR(surface, &pmodeCount, nullptr);
    details.presentationModes.resize(pmodeCount);
    device.getSurfacePresentModesKHR(surface, &pmodeCount, details.presentationModes.data());

    return details;
}

SurfaceFormatKHR VulkanRenderer::getOptimalSurfaceFormat(const vector<SurfaceFormatKHR> &formats) {
    // this means all formats are supported.
    if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined) {
        // normally the best formats to use
        return {vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto &format : formats) {
        // find the ones that are regarded as optimal
        if((format.format == vk::Format::eR8G8B8A8Unorm || format.format == vk::Format::eB8G8R8A8Unorm)
        && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return format;
        }
    }

    // pray it works
    return formats[0];
}

PresentModeKHR VulkanRenderer::getOptimalPresentationMode(const vector<vk::PresentModeKHR> &presentModes) {

    for (const auto &mode : presentModes) {
        // find the ones that are regarded as optimal
        if(mode == vk::PresentModeKHR::eMailbox) {
            return mode;
        }
    }

    // should be available on every system.
    return vk::PresentModeKHR::eFifo;
}

Extent2D VulkanRenderer::getSwapExtent(const SurfaceCapabilitiesKHR& surfaceCapabilities) {
    Extent2D currentExtent = surfaceCapabilities.currentExtent;
    
    // vulkan docs says if its equal to the max of uint32, it's of variable size.
    if(currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return currentExtent;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    Extent2D extent = Extent2D{static_cast<uint32_t>(w), static_cast<uint32_t>(h)};

    // clamp based on max and min value
    extent.width = std::min(extent.width, surfaceCapabilities.maxImageExtent.width);
    extent.height = std::min(extent.height, surfaceCapabilities.maxImageExtent.height);

    extent.width = std::max(extent.width, surfaceCapabilities.minImageExtent.width);
    extent.height = std::max(extent.height, surfaceCapabilities.minImageExtent.height);

    return extent;

}

void VulkanRenderer::cleanup() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(mainDevice.logical, nullptr);

    vkDestroyInstance(instance, nullptr);
}





