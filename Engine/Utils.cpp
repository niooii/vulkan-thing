#include "Utils.h"

namespace Engine {

    std::vector<char> Utils::ReadFile(const std::string_view& filename){
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error(std::format("failed to open file with name {}", filename));
        }

        size_t fsize = static_cast<size_t>(file.tellg());
        std::vector<char> buf(fsize);

        file.seekg(0);

        file.read(buf.data(), fsize);

        file.close();

        return buf;
    }
}