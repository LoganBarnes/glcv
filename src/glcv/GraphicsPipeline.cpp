// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "GraphicsPipeline.hpp"
#include "glcv/ErrorCheck.hpp"
#include "glcv/GLCV.hpp"
#include "glcv/util/file_util.h"
#include "glcv/util/map_util.h"
#include "glcv/util/vulkan_util.h"
#include "glcv/detail/ShaderManager.hpp"

#include <iostream>

namespace glcv {

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<GLCV> glcv,
                                   const std::vector<std::pair<std::string, vk::ShaderStageFlagBits>> &spirv_files,
                                   uint32_t width,
                                   uint32_t height)
//    : glcv_(std::move(glcv))
{
    // RENDER PASSES
    auto color_attachment = vk::AttachmentDescription()
                                .setFormat(glcv->image_format())
                                .setSamples(vk::SampleCountFlagBits::e1)
                                .setLoadOp(vk::AttachmentLoadOp::eClear)
                                .setStoreOp(vk::AttachmentStoreOp::eStore)
                                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                                .setInitialLayout(vk::ImageLayout::eUndefined)
                                .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    auto color_attachment_ref
        = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    auto subpass = vk::SubpassDescription()
                       .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                       .setColorAttachmentCount(1)
                       .setPColorAttachments(&color_attachment_ref);

    auto render_pass_info = vk::RenderPassCreateInfo()
                                .setAttachmentCount(1)
                                .setPAttachments(&color_attachment)
                                .setSubpassCount(1)
                                .setPSubpasses(&subpass);

    render_pass_ = glcv::util::make_shared_vk_object(glcv->device().createRenderPass(render_pass_info),
                                                     [device = glcv->device()](vk::RenderPass *p) {
                                                         device.destroy(*p);
                                                         DEBUG_PRINT("RenderPass destroyed");
                                                         delete p;
                                                     });
    DEBUG_PRINT("RenderPass created");

    // SHADERS
    std::vector<std::shared_ptr<vk::ShaderModule>> shader_modules;
    std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_info;

    for (const auto &spirv_file : spirv_files) {

        std::vector<char> spirv_data = util::read_file(spirv_file.first);

        auto shader_module_info = vk::ShaderModuleCreateInfo()
                                      .setCodeSize(spirv_data.size())
                                      .setPCode(reinterpret_cast<uint32_t *>(spirv_data.data()));

        shader_modules.emplace_back(
            glcv::util::make_shared_vk_object(glcv->device().createShaderModule(shader_module_info),
                                              [device = glcv->device()](vk::ShaderModule *p) {
                                                  device.destroy(*p);
                                                  DEBUG_PRINT("ShaderModule destroyed");
                                                  delete p;
                                              }));
        DEBUG_PRINT("ShaderModule created");

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
                              .setPrimitiveRestartEnable(vk::Bool32(false));

    auto viewport = vk::Viewport()
                        .setX(0.f)
                        .setY(0.f)
                        .setWidth(static_cast<float>(width))
                        .setHeight(static_cast<float>(height))
                        .setMinDepth(0.f)
                        .setMaxDepth(1.f);

    auto scissor = vk::Rect2D().setOffset({0, 0}).setExtent({width, height});

    auto viewport_info = vk::PipelineViewportStateCreateInfo()
                             .setViewportCount(1)
                             .setPViewports(&viewport)
                             .setScissorCount(1)
                             .setPScissors(&scissor);

    auto rasterizer_info = vk::PipelineRasterizationStateCreateInfo()
                               .setDepthClampEnable(vk::Bool32(false))
                               .setRasterizerDiscardEnable(vk::Bool32(false))
                               .setPolygonMode(vk::PolygonMode::eFill)
                               .setLineWidth(1.0f)
                               .setCullMode(vk::CullModeFlagBits::eBack)
                               .setFrontFace(vk::FrontFace::eCounterClockwise)
                               .setDepthBiasEnable(vk::Bool32(false))
                               .setDepthBiasConstantFactor(0.0f)
                               .setDepthBiasClamp(0.0f)
                               .setDepthBiasSlopeFactor(0.0f);

    auto multisample_info = vk::PipelineMultisampleStateCreateInfo()
                                .setSampleShadingEnable(vk::Bool32(false))
                                .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                                .setMinSampleShading(1.0f)
                                .setPSampleMask(nullptr)
                                .setAlphaToCoverageEnable(vk::Bool32(false))
                                .setAlphaToOneEnable(vk::Bool32(false));

    auto color_blend_attachment
        = vk::PipelineColorBlendAttachmentState()
              .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                 | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
              .setBlendEnable(vk::Bool32(true))
              .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
              .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
              .setColorBlendOp(vk::BlendOp::eAdd)
              .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
              .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
              .setAlphaBlendOp(vk::BlendOp::eAdd);

    auto color_blend_info = vk::PipelineColorBlendStateCreateInfo()
                                .setLogicOpEnable(vk::Bool32(false))
                                .setLogicOp(vk::LogicOp::eCopy)
                                .setAttachmentCount(1)
                                .setPAttachments(&color_blend_attachment)
                                .setBlendConstants(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});

    auto pipeline_layout_info = vk::PipelineLayoutCreateInfo()
                                    .setSetLayoutCount(0)
                                    .setPSetLayouts(nullptr)
                                    .setPushConstantRangeCount(0)
                                    .setPPushConstantRanges(nullptr);

    layout_ = glcv::util::make_shared_vk_object(glcv->device().createPipelineLayout(pipeline_layout_info),
                                                [device = glcv->device()](vk::PipelineLayout *p) {
                                                    device.destroy(*p);
                                                    DEBUG_PRINT("PipelineLayout destroyed");
                                                    delete p;
                                                });
    DEBUG_PRINT("PipelineLayout created");

    // PIPELINE
    auto pipeline_info
        = vk::GraphicsPipelineCreateInfo()
              .setStageCount(2)
              .setPStages(shader_stage_info.data())
              // We start by referencing the array of VkPipelineShaderStageCreateInfo structs.

              .setPVertexInputState(&vertex_input)
              .setPInputAssemblyState(&input_assembly)
              .setPViewportState(&viewport_info)
              .setPRasterizationState(&rasterizer_info)
              .setPMultisampleState(&multisample_info)
              .setPDepthStencilState(nullptr)
              .setPColorBlendState(&color_blend_info)
              .setPDynamicState(nullptr)
              // Then we reference all of the structures describing the fixed-function stage.

              .setLayout(*layout_)
              // After that comes the pipeline layout, which is a Vulkan handle rather than a struct pointer.

              .setRenderPass(*render_pass_)
              .setSubpass(0)
              // And finally we have the reference to the render pass and the index of the sub pass where this
              // graphics pipeline will be used. It is also possible to use other render passes with this pipeline
              // instead of this specific instance, but they have to be compatible with renderPass. The requirements
              // for compatibility are described here, but we won't be using that feature in this tutorial.

              .setBasePipelineHandle(nullptr)
              .setBasePipelineIndex(-1);

    pipeline_ = glcv::util::make_shared_vk_object(glcv->device().createGraphicsPipeline(nullptr, pipeline_info),
                                                  [glcv](vk::Pipeline *p) {
                                                      glcv->device().destroy(*p);
                                                      DEBUG_PRINT("Pipeline destroyed");
                                                      delete p;
                                                  });

    DEBUG_PRINT("Pipeline created");
}

} // namespace glcv
