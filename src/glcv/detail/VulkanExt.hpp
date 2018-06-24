// ////////////////////////////////////////////////////////////
// Created on 6/23/18.
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.hpp>

extern "C"
{
vk::Result vkExtInitInstance(vk::Instance instance);

}

//namespace glcv {
//namespace ext {

//VkResult createDebugReportCallbackEXT(vk::Instance instance,
//                                      const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
//                                      const VkAllocationCallbacks *pAllocator,
//                                      VkDebugReportCallbackEXT *pCallback);

//} // namespace glcv
//} // namespace ext