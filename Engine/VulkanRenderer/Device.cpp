#include <stdexcept>
#include "Device.h"

namespace Engine::Vulkan {

    Device::Device(Instance& instance, Surface& surface) : instance_(instance), surface_(surface) {
        PickPhysicalDevice();
        InitLogicalDevice();
        spdlog::debug("Vulkan device has been initialized.");
    }

    void Device::Destroy() {
        vkDestroyDevice(vk_logical_, nullptr);
    }

    VkDevice Device::vk_device() {
        return vk_logical_;
    }

    QueueFamilyIndicies& Device::queue_family_indices() {
        return queue_family_indices_;
    }

    SwapChainSupportDetails &Device::swapchain_support_details() {
        return swapchain_support_details_;
    }

    void Device::PickPhysicalDevice() {
        uint32_t num_devices;
        vkEnumeratePhysicalDevices(instance_.vk_instance(), &num_devices, nullptr);

        if(num_devices == 0) {
            throw std::runtime_error("failed to find vk_device.");
        }

        std::vector<VkPhysicalDevice> physical_devices{num_devices};
        vkEnumeratePhysicalDevices(instance_.vk_instance(), &num_devices, physical_devices.data());

        bool found_suitable_device{false};
        for(auto physical : physical_devices) {
            std::optional<std::string_view> device_name;
            if((device_name = DeviceSuitable(physical)).has_value()) {
                found_suitable_device = true;
                vk_physical_ = physical;
                spdlog::debug("Found first suitable device: {}", device_name.value()    );
                break;
            }
        }

        if(!found_suitable_device) {
            throw std::runtime_error("failed to find suitable vk_device.");
        }

    }

    void Device::InitLogicalDevice() {
        VkDeviceCreateInfo dev_create_info{};
        dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos = GetQueueCreateInfos(queue_family_indices_);

        dev_create_info.pQueueCreateInfos = queue_create_infos.data();
        dev_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

        // empty for now
        VkPhysicalDeviceFeatures dev_features{};

        dev_create_info.pEnabledFeatures = &dev_features;

        // device extensions

        dev_create_info.ppEnabledExtensionNames = required_device_extensions_.data();
        dev_create_info.enabledExtensionCount = static_cast<uint32_t>(
                required_device_extensions_.size()
                );



        VkResult result = vkCreateDevice(vk_physical_, &dev_create_info, nullptr, &vk_logical_);

        Utils::ExpectBadResult("Failed to create vk_device", result);

        vkGetDeviceQueue(vk_logical_, queue_family_indices_.graphics_family.value(),
                         0, &graphics_queue_handle);

        vkGetDeviceQueue(vk_logical_, queue_family_indices_.present_family.value(),
                         0, &present_queue_handle);
    }

    std::vector<VkDeviceQueueCreateInfo> Device::GetQueueCreateInfos(QueueFamilyIndicies &indices) {
        VkDeviceQueueCreateInfo graphics_create_info{};
        graphics_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphics_create_info.queueFamilyIndex = indices.graphics_family.value();
        graphics_create_info.queueCount = 1;

        float graphic_q_prio = 1.0f;
        graphics_create_info.pQueuePriorities = &graphic_q_prio;

        VkDeviceQueueCreateInfo present_create_info{};
        present_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        present_create_info.queueFamilyIndex = indices.present_family.value();
        present_create_info.queueCount = 1;

        float present_q_prio = 1.0f;
        present_create_info.pQueuePriorities = &present_q_prio;

        return std::vector<VkDeviceQueueCreateInfo> {
            graphics_create_info,
            present_create_info
        };
    }

    // Internal
    std::optional<std::string_view> Device::DeviceSuitable(VkPhysicalDevice physical_device) {
        // TODO! handle more later, for now choose first found that satisfies reqs

        VkPhysicalDeviceProperties device_properties{};
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);

        std::string_view this_device_name{device_properties.deviceName};

        bool has_required_extensions = HasRequiredExtensions(physical_device);

        if(!has_required_extensions) {
            return std::optional<std::string_view>{};
        }

        queue_family_indices_ = FindQueueFamilies(physical_device);
        swapchain_support_details_ = QuerySwapchainSupport(physical_device);

        bool device_suitable = queue_family_indices_.all_exist()
                && (!swapchain_support_details_.formats.empty() || !swapchain_support_details_.present_modes.empty());

        return device_suitable ? std::optional<std::string_view>{this_device_name} : std::optional<std::string_view>{};
    }

    bool Device::HasRequiredExtensions(VkPhysicalDevice physical_device) {
        uint32_t num_extensions;
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &num_extensions, nullptr);

        std::vector<VkExtensionProperties> available_extensions(num_extensions);
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &num_extensions, available_extensions.data());

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

    QueueFamilyIndicies Device::FindQueueFamilies(VkPhysicalDevice physical_device) {
        QueueFamilyIndicies indices;

        uint32_t num_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_families, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_families, queue_families.data());

        uint32_t i{0};
        for (const auto& queueFamily : queue_families) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }

            // no early return or else because the queues can overlap
            VkBool32 supports_presentation = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface_.vk_surface(), &supports_presentation);

            if (supports_presentation) {
                indices.present_family = i;
            }

            i++;
        }

        return indices;
    }

    SwapChainSupportDetails Device::QuerySwapchainSupport(VkPhysicalDevice physical_device) {
        SwapChainSupportDetails sc_details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface_.vk_surface(), &sc_details.capabilities);

        uint32_t num_formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface_.vk_surface(), &num_formats, nullptr);

        if (num_formats != 0) {
            sc_details.formats.resize(num_formats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface_.vk_surface(), &num_formats, sc_details.formats.data());
        }

        uint32_t num_present_modes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface_.vk_surface(), &num_present_modes, nullptr);

        if (num_present_modes != 0) {
            sc_details.present_modes.resize(num_present_modes);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface_.vk_surface(), &num_present_modes, sc_details.present_modes.data());
        }

        return sc_details;
    }
}