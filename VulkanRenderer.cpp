#include "VulkanRenderer.h"
#include "vulkan/vulkan.hpp"

using std::vector;
using globals::logger;

using namespace vk;

VulkanRenderer::VulkanRenderer() = default;
VulkanRenderer::~VulkanRenderer() = default;

Result VulkanRenderer::init(SDL_Window* windowTarget) {

    window = windowTarget;

    createInstance();
    
    return Result::eSuccess;
}

void VulkanRenderer::createInstance() {

    vk::ApplicationInfo appInfo{};
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
