#include "VulkanRenderer.h"
#include "Utils.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <vector>

using std::vector;
using globals::logger;

using namespace vk;

VulkanRenderer::VulkanRenderer() = default;
VulkanRenderer::~VulkanRenderer() = default;

Result VulkanRenderer::init(SDL_Window* windowTarget) {

    window = windowTarget;

    createInstance();
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
    extensionsAreSupported(&extensionNames);

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
        logger.err("No vulkan-supported device found.");
        throw std::runtime_error("GG");
    }

    // create queues
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physical);
    
    DeviceQueueCreateInfo qCreateInfo;
    qCreateInfo.sType = StructureType::eDeviceQueueCreateInfo;
    qCreateInfo.queueFamilyIndex = indices.graphicsFamily;
    float priority = 1.0f;
    qCreateInfo.pQueuePriorities = &priority;
    // just 1 for now
    qCreateInfo.queueCount = 1;


    // create logical device
    DeviceCreateInfo dCreateInfo{};
    dCreateInfo.sType = StructureType::eDeviceCreateInfo; 

    // a list of queues to create. in this case, jsut 1.
    dCreateInfo.queueCreateInfoCount = 1;  
    dCreateInfo.pQueueCreateInfos = &qCreateInfo;  

    dCreateInfo.enabledExtensionCount = 0;
    dCreateInfo.ppEnabledExtensionNames = nullptr;
    dCreateInfo.enabledLayerCount = 0;

    // physhical device features logical will use
    PhysicalDeviceFeatures pFeatures{};
    // none at t his time

    dCreateInfo.pEnabledFeatures = &pFeatures;

    // create the device
    mainDevice.logical = mainDevice.logical = mainDevice.physical.createDevice(dCreateInfo);
}

bool VulkanRenderer::validateDevice(PhysicalDevice& device) {
    PhysicalDeviceProperties properties = device.getProperties();
    PhysicalDeviceFeatures features = device.getFeatures();

    

    return true;
}

bool VulkanRenderer::extensionsAreSupported(vector<const char*>* extensionNames) {
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

        if(indices.isValid()) {
            break;
        }

        i++;
    }

    return indices;
}