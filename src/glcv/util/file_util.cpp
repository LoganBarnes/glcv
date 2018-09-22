// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "glcv/util/file_util.h"
#include <fstream>

namespace util {

std::vector<char> read_file(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    auto file_size = file.tellg();
    std::vector<char> buffer(static_cast<std::size_t>(file_size));

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();

    return buffer;
}

} // namespace util
