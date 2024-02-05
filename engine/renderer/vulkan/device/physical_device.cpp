#include "physical_device.h"

namespace Engine::Vulkan {

    PhysicalDevice::PhysicalDevice(VkPhysicalDevice vk_physical) : vk_physical_(vk_physical) {
        vkGetPhysicalDeviceProperties(vk_physical_, &vk_device_properties_);
    }

    std::string_view PhysicalDevice::name() {
        return {vk_device_properties_.deviceName};
    }

    VkPhysicalDevice PhysicalDevice::vk_physical() {
        return vk_physical_;
    }

    const std::vector<const char*>& PhysicalDevice::required_device_extensions() {
        return required_device_extensions_;
    }

    QueueFamilyIndices& PhysicalDevice::get_queue_families(Surface& surface) {

        if (queue_family_indices_.has_value()) {
            return queue_family_indices_.value();
        }

        QueueFamilyIndices& indices = queue_family_indices_.emplace();

        uint32_t num_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_, &num_queue_families, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_, &num_queue_families, queue_families.data());

        uint32_t i{0};
        for (const auto& queueFamily : queue_families) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }

            // no early return or else because the queues can overlap
            VkBool32 supports_presentation = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vk_physical_, i, surface.vk_surface(), &supports_presentation);

            if (supports_presentation) {
                indices.present_family = i;
            }

            i++;
        }

        return indices;
    }

    SwapchainSupportedProperties& PhysicalDevice::get_swapchain_support_properties(Surface& surface) {

        if (swapchain_support_properties_.has_value()) {
            return swapchain_support_properties_.value();
        }

        SwapchainSupportedProperties& sc_support_properties = swapchain_support_properties_.emplace();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physical_, surface.vk_surface(), &sc_support_properties.capabilities);

        uint32_t num_formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_, surface.vk_surface(), &num_formats, nullptr);

        if (num_formats != 0) {
            sc_support_properties.formats.resize(num_formats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_, surface.vk_surface(), &num_formats, sc_support_properties.formats.data());
        }

        uint32_t num_present_modes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_, surface.vk_surface(), &num_present_modes, nullptr);

        if (num_present_modes != 0) {
            sc_support_properties.present_modes.resize(num_present_modes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_, surface.vk_surface(), &num_present_modes, sc_support_properties.present_modes.data());
        }

        return sc_support_properties;
    }

    bool PhysicalDevice::HasRequiredExtensions() {
        uint32_t num_extensions;
        vkEnumerateDeviceExtensionProperties(vk_physical_, nullptr, &num_extensions, nullptr);

        std::vector<VkExtensionProperties> available_extensions(num_extensions);
        vkEnumerateDeviceExtensionProperties(vk_physical_, nullptr, &num_extensions, available_extensions.data());

        std::set<std::string_view> extensions_to_find(
                required_device_extensions_.begin(),
                required_device_extensions_.end()
        );

        for(const auto& extension : available_extensions) {
            // implicit conversions smh
            extensions_to_find.erase(extension.extensionName);
        }

        return extensions_to_find.empty();
    }

    bool PhysicalDevice::IsSuitable(Surface& surface) {
        bool has_required_extensions = HasRequiredExtensions();

        if(!has_required_extensions) {
            return false;
        }

        QueueFamilyIndices& queue_family_indices_ = get_queue_families(surface);
        SwapchainSupportedProperties& swapchain_support_details_ = get_swapchain_support_properties(surface);

        bool device_suitable = queue_family_indices_.all_exist()
                               && (!swapchain_support_details_.formats.empty() || !swapchain_support_details_.present_modes.empty());

        return device_suitable;
    }

}