#pragma once

#include <iostream>

#define VK_CHECK(x) \
    do { \
        VkResult err = x; \
        if (err) { \
            std::cout << "detected vulkan error: " << err << std::endl; \
            abort(); \
        } \
    } while (0)

class VulkanUtilities
{
};
