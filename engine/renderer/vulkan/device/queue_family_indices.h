#ifndef VULKAN_STUFF_QUEUE_FAMILY_INDICES_H
#define VULKAN_STUFF_QUEUE_FAMILY_INDICES_H

#include <optional>
#include <cstdint>

namespace Engine::Vulkan {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool all_exist() const {
            return graphics_family.has_value() && present_family.has_value();
        }

    };

}

#endif