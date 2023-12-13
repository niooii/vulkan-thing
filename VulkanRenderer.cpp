#include "VulkanRenderer.h"

using std::vector;
using globals::logger;

VulkanRenderer::VulkanRenderer() = default;
VulkanRenderer::~VulkanRenderer() = default;

vk::Result VulkanRenderer::init(SDL_Window* windowTarget) {

    window = windowTarget;

    createInstance();
    
    return vk::Result::eSuccess;
}

void VulkanRenderer::createInstance() {
    using vk::StructureType;
    using vk::Result;

    vk::ApplicationInfo appInfo{};
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "Vulkan app woo";
    appInfo.applicationVersion = vk::makeApiVersion(1, 0, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = vk::makeApiVersion(1, 0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo{};
    createInfo.sType = StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;

    unsigned int extensionCount;
    vector<const char*> extensionNames{};

    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

//    std::cout << extensionNames.data() << '\n';

    createInfo.ppEnabledExtensionNames = extensionNames.data();
    createInfo.enabledExtensionCount = extensionNames.size();

    vk::Result result;
    if ((result = vk::createInstance(&createInfo, nullptr, &instance)) != Result::eSuccess) {
        logger.err("Failed to create vulkan instance. Error: ");
        logger.err(vk::to_string(result).c_str());
        throw std::runtime_error("failed to create instance!");
    }

    logger.debug("Vulkan instance successfully initialized.");
}
