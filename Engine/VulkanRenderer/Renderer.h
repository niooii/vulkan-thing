#ifndef VULKAN_STUFF_RENDERER_H
#define VULKAN_STUFF_RENDERER_H

#include "Instance.h"
#include "Swapchain.h"
#include "Surface.h"
#include "GraphicsPipeline.h"
#include "Device/Device.h"
#include "Device/DeviceUtils.h"
#include "Utils.h"

#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace Engine::Vulkan {

    class Renderer {
    public:
        // The parameters passed in are used to create the underlying Vulkan instance_.
        Renderer(const char* application_name, const char* engine_name, Window &window, bool validation_layers_enabled);
        // Destroys all resources in *deliberate* order.
        ~Renderer();
    private:
        std::optional<Instance> instance_;
        std::optional<Surface> surface_;
        std::optional<Device> device_;
        std::optional<Swapchain> swapchain_;
        std::optional<GraphicsPipeline> pipeline_;
    };

}

#endif
