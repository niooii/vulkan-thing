#pragma once

using std::vector;
using namespace vk;

const vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    // if -1, they do not exist.
    int graphicsFamily = -1;
    int presentationFamily = -1;

    // check if queue families exist
    bool isValid() {
        return graphicsFamily >= 0 && presentationFamily;
    }
};

struct SwapChainDetails {
    SurfaceCapabilitiesKHR surfaceCapabilities;      // surface properties, like iamge size / extent
    vector<SurfaceFormatKHR> formats;                // surface image formats, like rgb or rgba 1.0 or 255 etc
    vector<PresentModeKHR> presentationModes;        // how images presented
};