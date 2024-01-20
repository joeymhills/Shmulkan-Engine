#pragma once

#include <vulkan/vulkan_core.h>
#include <optional>
#include <vector>

namespace ShmulkDevice {

    void pickPhysicalDevice(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, VkSampleCountFlagBits* msaaSamples, const std::vector<const char*> deviceExtensions);

    void createLogicalDevice(VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkQueue* presentQueue, VkDevice* device, VkSurfaceKHR* surface,
        const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers);

    bool checkDeviceExtensionSupport(VkPhysicalDevice* device, const std::vector<const char*> deviceExtensions);

    bool isDeviceSuitable(VkPhysicalDevice* device, const std::vector<const char*> deviceExtensions);

    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice* physicalDevice);
}
