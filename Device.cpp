#include <vulkan/vulkan_core.h>

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <set>

#include "Debug.h"
#include "Device.h"
#include "Utility.h"
#include "Swapchain.h"

namespace ShmulkDevice {
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }


    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions) {
        ShmulkUtilities::QueueFamilyIndices indices = ShmulkUtilities::findQueueFamilies(device, surface);

        bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            ShmulkSwapchain::SwapChainSupportDetails swapChainSupport = ShmulkSwapchain::querySwapChainSupport(device, &surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice* physicalDevice) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(*physicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts
        & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    void pickPhysicalDevice(VkInstance* instance, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface, VkSampleCountFlagBits* msaaSamples, const std::vector<const char*> deviceExtensions) {
        uint32_t deviceCount = 0;

        vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device, *surface, deviceExtensions)) {
                *physicalDevice = device;
                *msaaSamples = getMaxUsableSampleCount(physicalDevice);
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void createLogicalDevice(VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkQueue* presentQueue, VkDevice* device, VkSurfaceKHR* surface,
                    const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers) {
    
        ShmulkUtilities::QueueFamilyIndices indices = ShmulkUtilities::findQueueFamilies(*physicalDevice, *surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures. sampleRateShading = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (ShmulkDebug::enableValidationLayers()) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(*physicalDevice, &createInfo, nullptr, device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(*device, indices.graphicsFamily.value(), 0, graphicsQueue);
        vkGetDeviceQueue(*device, indices.presentFamily.value(), 0, presentQueue);
    }

}
