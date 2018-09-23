// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "ShaderManager.hpp"

namespace glcv {

const std::unordered_map<std::string, vk::ShaderStageFlagBits> &shader_ext_to_type()
{
    static std::unordered_map<std::string, vk::ShaderStageFlagBits>
        ext_map{{".vert", vk::ShaderStageFlagBits::eVertex},
                {".tesc", vk::ShaderStageFlagBits::eTessellationControl},
                {".tese", vk::ShaderStageFlagBits::eTessellationEvaluation},
                {".geom", vk::ShaderStageFlagBits::eGeometry},
                {".frag", vk::ShaderStageFlagBits::eFragment},
                {".comp", vk::ShaderStageFlagBits::eCompute}};
    return ext_map;
}

} // namespace glcv
