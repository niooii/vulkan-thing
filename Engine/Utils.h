#ifndef VULKAN_STUFF_UTILS_H
#define VULKAN_STUFF_UTILS_H

#include <fstream>
#include <vector>
#include <vulkan/vk_enum_string_helper.h>

namespace Engine {

    namespace Utils {

        static std::vector<char> ReadFile(const std::string_view& filename);
        static void ExpectBadResult(const char* message, VkResult res);

    };

}

#endif
