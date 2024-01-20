#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN

#include <vector>

namespace ShmulkSwapchain {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR* surface);
}
