// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "GLCV.hpp"
#include <stdexcept>

//namespace glcv {

std::vector<VkExtensionProperties> GLCV::get_available_extensions()
{
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    return extensions;
}

void GLCV::init(const std::string &app_name, const std::vector<const char *> &extensions_names)
{
    create_instance(app_name, extensions_names);
}

void GLCV::create_instance(const std::string &app_name, const std::vector<const char *> &extensions_names)
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
    instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions_names.size());
    instance_info.ppEnabledExtensionNames = (extensions_names.empty() ? nullptr : extensions_names.data());
    instance_info.enabledLayerCount = 0;
    instance_info.ppEnabledLayerNames = nullptr;

    auto &instance = GLCV::self().instance_;
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

GLCV::GLCV() = default;
GLCV::~GLCV() = default;

GLCV &GLCV::self()
{
    static GLCV glcv;
    return glcv;
}

//} // namespace glcv
