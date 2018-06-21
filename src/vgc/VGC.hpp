// ////////////////////////////////////////////////////////////
// Vulkan Graphics Classes
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>

//namespace vgc
//{
template <typename T, typename D, D Deleter>
struct stateless_deleter
{
    typedef T pointer;

    void operator()(T x) { Deleter(x); }
};

class VGC
{
public:
    static std::vector<VkExtensionProperties> get_available_extensions();

    // TODO: add extensions input
    static void init(const std::string &app_name = "VGC Application");
    static void create_instance(const std::string &app_name = "VGC Application");

    VGC(const VGC &) = delete;
    VGC(VGC &&) noexcept = delete;
    VGC &operator=(const VGC &) = delete;
    VGC &operator=(VGC &&) noexcept = delete;

private:
    VGC();
    ~VGC();

    static VGC &self();

    std::shared_ptr<VkInstance> instance_;
};

//} // namespace vgc
