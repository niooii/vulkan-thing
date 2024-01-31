#ifndef VULKAN_STUFF_UTILS_H
#define VULKAN_STUFF_UTILS_H

#include <fstream>
#include <vector>

namespace Engine {

    namespace Utils {

        static std::vector<char> ReadFile(const std::string_view& filename);

    };

}

#endif
