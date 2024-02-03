#ifndef VULKAN_STUFF_SWAPCHAINSUPPORTEDPROPERTIES_H
#define VULKAN_STUFF_SWAPCHAINSUPPORTEDPROPERTIES_H

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
