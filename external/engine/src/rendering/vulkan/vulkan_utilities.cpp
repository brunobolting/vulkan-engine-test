#include "vulkan_utilities.h"
#include <fstream>
#include <vector>

namespace ZERO
{
    bool VulkanUtilities::LoadShaderModule(const std::string &filePath, VkDevice device, VkShaderModule &outShaderModule) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        size_t filesize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));
        file.seekg(0);
        file.read((char*) buffer.data(), filesize);
        file.close();

        VkShaderModuleCreateInfo createInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = buffer.size() * sizeof(uint32_t);
        createInfo.pCode = buffer.data();
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            return false;
        }
        outShaderModule = shaderModule;
        return true;
    }
}
