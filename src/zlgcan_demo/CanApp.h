#ifndef __CANAPP_H
#define __CANAPP_H
// header for gui
#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>  // printf, fprintf
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// Volk headers
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
#define VOLK_IMPLEMENTATION
#include <volk.h>
#endif
// std headers
#include <iostream>
#include <vector>
// header for zlgcan
#include "zlgcan.h"
#include "zdbc/zdbc.h"
// header for open directory
#include "nfd.h"

class CanApp
{
public:
// #define APP_USE_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
    VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
#endif
    // Data
    VkAllocationCallbacks *g_Allocator = nullptr;
    VkInstance g_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice g_Device = VK_NULL_HANDLE;
    uint32_t g_QueueFamily = (uint32_t)-1;
    VkQueue g_Queue = VK_NULL_HANDLE;
    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
    ImGui_ImplVulkanH_Window g_MainWindowData;
    uint32_t g_MinImageCount = 2;
    bool g_SwapChainRebuild = false;
    bool show_main_window = true;
    GLFWwindow *window = nullptr;
    ImGui_ImplVulkanH_Window *wd = nullptr;
    bool show_demo_window = true;
    bool main_is_minimized = false;
    DBCHandle dbch = 0;

    CanApp() {};
    ~CanApp() {};

    int InitGUIS();
    int InitCANLibs();
    bool IsExit();
    int WindowResized();
    void RenderGUI();
    void RenderMainWindow();
    void RenderPlatformWindow();
    void Close();
    bool IsExtensionAvailable(const ImVector<VkExtensionProperties> &properties, const char *extension);
    void SetupVulkan(ImVector<const char *> instance_extensions);
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height);
    void CleanupVulkan();
    void CleanupVulkanWindow();
    void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window *wd);
};

#endif