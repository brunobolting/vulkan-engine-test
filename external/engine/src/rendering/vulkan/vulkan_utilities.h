#pragma once

#include <iostream>
#include <vulkan/vulkan.h>

#define VK_CHECK(x) \
    do { \
        VkResult err = x; \
        if (err) { \
            std::cout << "detected vulkan error: " << err << std::endl; \
            abort(); \
        } \
    } while (0)

namespace ZERO
{
    class VulkanUtilities
    {
        public:
            static bool LoadShaderModule(const std::string &filePath, VkDevice device, VkShaderModule &outShaderModule);
    };

}
