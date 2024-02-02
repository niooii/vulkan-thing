#ifndef VULKAN_STUFF_PHYSICALDEVICE_H
#define VULKAN_STUFF_PHYSICALDEVICE_H


namespace Engine::Vulkan {

    class PhysicalDevice {
    public:
        explicit PhysicalDevice(VkPhysicalDevice vk_physical);
        std::string_view name();

    private:
        VkPhysicalDevice vk_physical_;
        VkPhysicalDeviceProperties vk_device_properties_;
    };

}


#endif
