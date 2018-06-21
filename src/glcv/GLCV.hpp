// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>

//namespace glcv
//{
template <typename T, typename D, D Deleter>
struct stateless_deleter
{
    typedef T pointer;

    void operator()(T x) { Deleter(x); }
};

class GLCV
{
public:
    static std::vector<VkExtensionProperties> get_available_extensions();

    // TODO: add extensions input
    static void init(const std::string &app_name = "GLCV Application");
    static void create_instance(const std::string &app_name = "GLCV Application");

    GLCV(const GLCV &) = delete;
    GLCV(GLCV &&) noexcept = delete;
    GLCV &operator=(const GLCV &) = delete;
    GLCV &operator=(GLCV &&) noexcept = delete;

private:
    GLCV();
    ~GLCV();

    static GLCV &self();

    std::shared_ptr<VkInstance> instance_;
};

//} // namespace glcv
