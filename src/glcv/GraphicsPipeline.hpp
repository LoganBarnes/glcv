// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <glcv/GLCVForwardDeclarations.hpp>
#include <vulkan/vulkan.hpp>
#include <memory>
#include <vector>

namespace glcv {

class GraphicsPipeline
{
public:
    GraphicsPipeline(std::shared_ptr<GLCV> glcv,
                     const std::vector<std::pair<std::string, vk::ShaderStageFlagBits>> &spirv_files,
                     uint32_t width,
                     uint32_t height);

private:
    //    std::shared_ptr<GLCV> glcv_;
    std::shared_ptr<vk::RenderPass> render_pass_;
    std::shared_ptr<vk::PipelineLayout> layout_;
    std::shared_ptr<vk::Pipeline> pipeline_;
};

} // namespace glcv
