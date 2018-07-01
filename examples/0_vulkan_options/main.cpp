// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#include "loop/SimpleLoop.hpp"
#include "glcv/GLCV.hpp"
#include "glcv/util/vector_util.hpp"
#include "glcv/util/map_util.hpp"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui.h>

#ifndef NDEBUG
constexpr bool debug = true;
#else
constexpr bool debug = false;
#endif

struct Extensions
{
    std::map<std::string, vk::ExtensionProperties> available_extensions_;
    std::vector<std::string> not_selected_extensions_;
    std::vector<std::string> selected_extensions_;
    std::vector<const char *> required_extensions_;
};

//struct Layers {
//    std::map<const char*, vk::ExtensionProperties> available_extensions_;
//    std::vector<const char*> not_selected_extensions_;
//    std::vector<const char*> selected_extensions_;
//    std::vector<const char*> required_extensions_;
//};

class VulkanOptions : public examples::SimpleLoop
{
public:
    VulkanOptions() : examples::SimpleLoop("Wire Cube Example", 640, 480, true, true)
    {
        {
            auto extensions = vk::enumerateInstanceExtensionProperties();
            for (auto &ext : extensions) {
                extensions_.available_extensions_.emplace(ext.extensionName, ext);
                extensions_.not_selected_extensions_.emplace_back(ext.extensionName);
            }
            extensions_.required_extensions_ = get_required_extensions();
            for (auto &ext : extensions_.required_extensions_) {
                extensions_.selected_extensions_.emplace_back(ext);
            }
        }

        util::print_vector("Available layers:", vk::enumerateInstanceLayerProperties(), [](auto &layer) {
            return layer.layerName;
        });

        auto surf_func = [&](VkInstance instance, VkSurfaceKHR *surface) {
            return glfwCreateWindowSurface(instance, get_vulkan_window(), nullptr, surface);
        };
        glcv_ = glcv::make_glcv("Example", debug, get_required_extensions(), {}, surf_func);
        // glcv_ = glcv::make_glcv("Example", debug, get_required_extensions());

        vk::PhysicalDeviceProperties device_props;
        util::print_vector("\nAvailable devices:", glcv_->instance().enumeratePhysicalDevices(), [&](auto &device) {
            device.getProperties(&device_props);
            return device_props.deviceName;
        });
    }

    void update(float /*sim_time*/, float /*time_step*/) final
    {
        if (!glcv_->surface()) {
            --counter_;
        }

        if (counter_ == 0) {
            glfwSetWindowShouldClose(get_vulkan_window(), GLFW_TRUE);
        }
    }

    void render(int /*view_width*/, int /*view_height*/, float) const final {}

    void render_gui(int view_width, int view_height) const final
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(view_width, view_height));
        ImGui::Begin("No Title", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        ImGui::Text("Press ESC to quit (or close either window)");

        if (ImGui::CollapsingHeader("Vulkan Options", "vulkan_options", true, true)) {
            if (ImGui::TreeNode("Extensions")) {

                ImGui::Columns(3, nullptr);

                ImGui::Text("AVAILABLE:");
                for (auto &not_selected_extension : extensions_.not_selected_extensions_) {
                    if (ImGui::Selectable(not_selected_extension.c_str())) {
                    }
                }

                ImGui::NextColumn();
                ImGui::Text("SELECTED:");
                for (auto &selected_extension : extensions_.selected_extensions_) {
                    if (ImGui::Selectable(selected_extension.c_str())) {
                    }
                }

                ImGui::NextColumn();
                ImGui::Text("REQUIRED (GLFW):");
                ImGui::Separator();
                for (auto &selected_extension : extensions_.required_extensions_) {
                    if (ImGui::Selectable(selected_extension)) {
                    }
                }

                ImGui::Columns(1);
                ImGui::TreePop();
            }
        }

        ImGui::End();
    };

private:
    glcv::GLCV glcv_;
    unsigned counter_ = 10000000;

    Extensions extensions_;
};

int main()
{
    VulkanOptions example;
    example.run_loop();
    return 0;
}
