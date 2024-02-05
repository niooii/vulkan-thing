#ifndef VULKAN_STUFF_SWAPCHAIN_SUPPORTED_PROPERTIES_H
#define VULKAN_STUFF_SWAPCHAIN_SUPPORTED_PROPERTIES_H

#include <vulkan/vulkan.h>

#include <vector>

namespace Engine::Vulkan {

    struct SwapchainSupportedProperties {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

}


#endif
