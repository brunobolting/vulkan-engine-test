#include "vulkan_buffer.h"
#include <cstring>

namespace ZERO
{
    /*
     * VULKAN BUFFER
    */
    void VulkanBuffer::CopyBuffer(
        VkDevice *device,
        VkCommandPool *commandPool,
        VkQueue *queue,
        VulkanBuffer *srcBuffer,
        VulkanBuffer *dstBuffer
    ) {
        // create buufer if not correct size
        if (dstBuffer->_bufferSize != srcBuffer->GetBufferSize() || dstBuffer->_buffer == VK_NULL_HANDLE) {
            dstBuffer->createBuffer(srcBuffer->GetBufferSize());
        }

        VkCommandBufferAllocateInfo allocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        allocateInfo.commandPool = *commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(*device, &allocateInfo, &commandBuffer);

        // record copy command into command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

        VkBufferCopy copyRegion {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = srcBuffer->GetBufferSize()
        };

        vkCmdCopyBuffer(commandBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &copyRegion);
        vkEndCommandBuffer(commandBuffer);
        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(*queue, 1, &submitInfo, VK_NULL_HANDLE);

        // TODO: this should probably be removed
        vkQueueWaitIdle(*queue);

        vkFreeCommandBuffers(*device, *commandPool, 1, &commandBuffer);
    }

    VulkanBuffer::VulkanBuffer(VmaAllocator *allocator, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage)
    : _allocator {allocator}, _usageFlags(usageFlags), _memoryUsage(memoryUsage) {}

    VulkanBuffer::~VulkanBuffer() {
        clearBuffer();
    }

    void VulkanBuffer::UploadData(int *data, size_t bufferSize) {
        if (_bufferSize != bufferSize || _buffer == VK_NULL_HANDLE) {
            createBuffer(bufferSize);
        }
        void *tempData;
        vmaMapMemory(*_allocator, _allocation, &tempData);
        memcpy(tempData, data, bufferSize);
        vmaUnmapMemory(*_allocator, _allocation);
    }

    void VulkanBuffer::createBuffer(size_t bufferSize) {
        clearBuffer();
        _bufferSize = bufferSize;
        VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferCreateInfo.size = _bufferSize;
        bufferCreateInfo.usage = _usageFlags;
        VmaAllocationCreateInfo allocationCreateInfo = {};
        allocationCreateInfo.usage = _memoryUsage;
        auto ok = vmaCreateBuffer(*_allocator, &bufferCreateInfo, &allocationCreateInfo, &_buffer, &_allocation, nullptr);
        if (ok != VK_SUCCESS) {
            printf("failed to allocate buffer\n");
        }
    }

    void VulkanBuffer::clearBuffer() {
        if (_buffer == VK_NULL_HANDLE) {
            return;
        }
        vmaDestroyBuffer(*_allocator, _buffer, _allocation);
        _buffer = VK_NULL_HANDLE;
        _allocation = VK_NULL_HANDLE;
        _bufferSize = 0;
    }

    /*
     * VERTEX BUFFER
    */
    VulkanVertexBuffer::VulkanVertexBuffer(VulkanRenderer *renderer) : _renderer(renderer) {
        _buffer = std::make_shared<VulkanBuffer>(
            &_renderer->_allocator,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY
        );
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
        if (_buffer) {
            _buffer.reset();
            _buffer = nullptr;
        }
    }

    void VulkanVertexBuffer::Bind() {
        if (_buffer && _buffer->IsAllocated()) {
            VkDeviceSize offset { 0 };
            VkBuffer buffers[1] { _buffer->GetBuffer() };
            vkCmdBindVertexBuffers(_renderer->_mainCommandBuffer, 0, 1, buffers, &offset);
        }
    }

    void VulkanVertexBuffer::UploadData(const std::vector<Vertex> &vertices) {
        auto bufferSize = vertices.size() * sizeof(Vertex);
        auto stagingBuffer = std::make_shared<VulkanBuffer>(
            &_renderer->_allocator,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_MEMORY_USAGE_CPU_ONLY
        );
        stagingBuffer->UploadData((int*)vertices.data(), bufferSize);
        VulkanBuffer::CopyBuffer(
            &_renderer->_device,
            &_renderer->_commandPool,
            &_renderer->_graphicsQueue,
            stagingBuffer.get(),
            _buffer.get()
        );
    }

    /*
     * INDEX BUFFER
    */
    VulkanIndexBuffer::VulkanIndexBuffer(VulkanRenderer *renderer) : _renderer(renderer) {
        _buffer = std::make_shared<VulkanBuffer>(
            &_renderer->_allocator,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY
        );
    }

    VulkanIndexBuffer::~VulkanIndexBuffer() {
        if (_buffer) {
            _buffer.reset();
            _buffer = nullptr;
        }
    }

    void VulkanIndexBuffer::Bind() {
        if (_buffer && _buffer->IsAllocated()) {
            VkDeviceSize offset { 0 };
            vkCmdBindIndexBuffer(_renderer->_mainCommandBuffer, _buffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT32);
        }
    }

    void VulkanIndexBuffer::UploadData(const std::vector<uint32_t> &indices) {
        auto bufferSize = indices.size() * sizeof(uint32_t);
        _count = indices.size();
        auto stagingBuffer = std::make_shared<VulkanBuffer>(
            &_renderer->_allocator,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_MEMORY_USAGE_CPU_ONLY
        );
        stagingBuffer->UploadData((int*)indices.data(), bufferSize);
        VulkanBuffer::CopyBuffer(
            &_renderer->_device,
            &_renderer->_commandPool,
            &_renderer->_graphicsQueue,
            stagingBuffer.get(),
            _buffer.get()
        );
    }

    size_t VulkanIndexBuffer::GetCount() {
        return _count;
    }
}

