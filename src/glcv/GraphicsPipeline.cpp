// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "GraphicsPipeline.hpp"
#include "glcv/ErrorCheck.hpp"
#include "glcv/GLCV.hpp"
#include "glcv/util/file_util.h"
#include "glcv/util/map_util.h"
#include "glcv/detail/ShaderManager.hpp"

#include <iostream>

namespace glcv {

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<GLCV> glcv,
                                   const std::vector<std::pair<std::string, vk::ShaderStageFlagBits>> &spirv_files,
                                   uint32_t width,
                                   uint32_t height)
//    : glcv_(std::move(glcv))
{
    std::vector<std::shared_ptr<vk::ShaderModule>> shader_modules;
    std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_info;

    for (const auto &spirv_file : spirv_files) {

        std::vector<char> spirv_data = util::read_file(spirv_file.first);

        auto shader_module_info = vk::ShaderModuleCreateInfo()
                                      .setCodeSize(spirv_data.size())
                                      .setPCode(reinterpret_cast<uint32_t *>(spirv_data.data()));

        shader_modules.emplace_back(new vk::ShaderModule(nullptr), [glcv](vk::ShaderModule *p) {
            if (*p) {
                glcv->device().destroy(*p);
                DEBUG_PRINT("ShaderModule destroyed");
            }
            delete p;
        });

        GLCV_CHECK(glcv->device().createShaderModule(&shader_module_info, nullptr, shader_modules.back().get()));
        DEBUG_PRINT("ShaderModule created: ");

        shader_stage_info.emplace_back(vk::PipelineShaderStageCreateInfo()
                                           .setStage(spirv_file.second)
                                           .setModule(*shader_modules.back())
                                           .setPName("main"));
    }

    // VAO?
    auto vertex_input = vk::PipelineVertexInputStateCreateInfo()
                            .setVertexBindingDescriptionCount(0)
                            .setPVertexBindingDescriptions(nullptr)
                            .setVertexAttributeDescriptionCount(0)
                            .setPVertexAttributeDescriptions(nullptr);

    // GL_POINTS, GL_LINE_STRIP, GL_TRIANGLES
    auto input_assembly = vk::PipelineInputAssemblyStateCreateInfo()
                              .setTopology(vk::PrimitiveTopology::eTriangleList)
                              .setPrimitiveRestartEnable(false);

    auto viewport = vk::Viewport()
                        .setX(0.f)
                        .setY(0.f)
                        .setWidth(static_cast<float>(width))
                        .setHeight(static_cast<float>(height))
                        .setMinDepth(0.f)
                        .setMaxDepth(1.f);

    auto scissor = vk::Rect2D().setOffset({0, 0}).setExtent({width, height});

    // unused for now
    (void)vertex_input;
    (void)input_assembly;
    (void)viewport;
    (void)scissor;
}

} // namespace glcv
