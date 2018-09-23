// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.hpp>
#include <unordered_map>

namespace glcv {

const std::unordered_map<std::string, vk::ShaderStageFlagBits> &shader_ext_to_type();

} // namespace glcv
