#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>
#include <cstring>
#include "Utility.h"

namespace ShmulkInstance {

    void createInstance(std::vector<const char*> validationLayers, VkInstance* instance);
    
    void createSurface(VkInstance* instance, GLFWwindow* window, VkSurfaceKHR* surface);

} // namespace ShmulkInstance
