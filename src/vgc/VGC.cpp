// ////////////////////////////////////////////////////////////
// Vulkan Graphics Classes
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "VGC.hpp"
#include <stdexcept>

//namespace vgc {

std::vector<VkExtensionProperties> VGC::get_available_extensions()
{
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    return extensions;
}

void VGC::init(const std::string &app_name)
{
    create_instance(app_name);
}

void VGC::create_instance(const std::string &app_name)
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instance_info = {};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pNext = nullptr;
    instance_info.flags = 0;
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = 0;
    instance_info.ppEnabledExtensionNames = nullptr;
    instance_info.enabledLayerCount = 0;
    instance_info.ppEnabledLayerNames = nullptr;

    auto &instance = VGC::self().instance_;
    instance = std::shared_ptr<VkInstance>(new VkInstance(), [](auto p) {
        vkDestroyInstance(*p, nullptr);
        delete p;
    });

    VkResult res = vkCreateInstance(&instance_info, nullptr, instance.get());

    if (res != VK_SUCCESS) {
        // TODO: print res somehow?
        throw std::runtime_error("failed to create instance!");
    }
}

VGC::VGC() = default;
VGC::~VGC() = default;

VGC &VGC::self()
{
    static VGC vgc;
    return vgc;
}

//} // namespace vgc
