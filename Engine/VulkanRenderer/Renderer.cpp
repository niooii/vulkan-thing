#include "Renderer.h"

namespace Engine::Vulkan {

    Renderer::Renderer(const char* application_name, const char* engine_name, Window& window, bool validation_layers_enabled) {

        // init instance
        instance_.emplace(Instance{application_name, engine_name, window, validation_layers_enabled});
        Instance& instance = instance_.value();

        std::vector<PhysicalDevice> physical_devices = DeviceUtils::GetPhysicalDevices(instance);

        assert(!physical_devices.empty());

        device_

        spdlog::info("Finished renderer initialization.");
    }

    Renderer::~Renderer() {
        pipeline_.value().Destroy();
        swapchain_.value().Destroy();
        device_.value().Destroy();
        surface_.value().Destroy();
        instance_.value().Destroy();
    }

}