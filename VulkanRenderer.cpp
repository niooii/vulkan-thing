#include "VulkanRenderer.h"

// for tesitng
#include <iostream>

VulkanRenderer::VulkanRenderer() = default;

int VulkanRenderer::init(SDL_Window* windowTarget) {
    using vk::StructureType;
    using vk::Result;
    using std::vector;

    window = windowTarget;

    vk::ApplicationInfo appInfo{};
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo{};
    createInfo.sType = StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;
    
    unsigned int extensionCount;
    vector<const char*> extensionNames{};

    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());
    
    for(const char* n : extensionNames) {
        std::cout << n << '\n';
    }

    std::cout << extensionNames.data() << '\n';

    // if(status == SDL_FALSE) {
    //     std::cout << "the fuck" << '\n';
    // }

    createInfo.ppEnabledExtensionNames = extensionNames.data();
    createInfo.enabledExtensionCount = extensionNames.size();

    vk::Instance instance;
    vk::Result result;
    if ((result = vk::createInstance(&createInfo, nullptr, &instance)) != Result::eSuccess) {
        std::cout << vk::to_string(result) << '\n';
        // vk::to_string();
        throw std::runtime_error("failed to create instance!");
    }
    
    return 0;
}