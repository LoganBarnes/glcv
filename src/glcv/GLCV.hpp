// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <memory>
#include <vector>

class GLCV
{
public:
    static void init(const std::string &app_name = "GLCV Application",
                     const std::vector<const char *> &extension_names = {},
                     const std::vector<const char *> &layer_names = {},
                     bool set_debug_callback = false);

    static void destroy();

    static std::vector<vk::PhysicalDevice> get_available_devices();

    GLCV(const GLCV &) = delete;
    GLCV(GLCV &&) noexcept = delete;
    GLCV &operator=(const GLCV &) = delete;
    GLCV &operator=(GLCV &&) noexcept = delete;

private:
    GLCV();
    ~GLCV();

    static GLCV &self();

    std::shared_ptr<vk::Instance> instance_;
    std::shared_ptr<vk::DebugReportCallbackEXT> debug_callback_;

    void create_instance(const std::string &app_name,
                         const std::vector<const char *> &extension_names,
                         const std::vector<const char *> &layer_names);

    void setup_debug_callback();
};
