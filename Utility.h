#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

namespace ShmulkUtilities {

    std::vector<const char*> getRequiredExtensions();
 
}
