#pragma once

#include <engine/rendering/buffer.h>
#include "vulkan_renderer.h"

namespace ZERO
{
    struct VulkanBuffer {
        public:
            static void CopyBuffer(VkDevice *device, VkCommandPool *commandPool, VkQueue *queue, VulkanBuffer *srcBuffer, VulkanBuffer *dstBuffer);
        public:
            VulkanBuffer(VmaAllocator *allocator, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage);
            ~VulkanBuffer();
            void UploadData(int* data, size_t bufferSize);
            [[nodiscard]] VkBuffer GetBuffer() const { return _buffer; };
            [[nodiscard]] VmaAllocation GetAllocation() const { return _allocation; };
            [[nodiscard]] size_t GetBufferSize() const { return _bufferSize; };
            [[nodiscard]] inline bool IsAllocated() const { return _buffer != VK_NULL_HANDLE; };

        private:
            void createBuffer(size_t bufferSize);
            void clearBuffer();
            VmaAllocator *_allocator { nullptr };
            VkBufferUsageFlags _usageFlags;
            VmaMemoryUsage _memoryUsage;
            size_t _bufferSize { 0 };
            VkBuffer _buffer { VK_NULL_HANDLE };
            VmaAllocation _allocation { VK_NULL_HANDLE };
    };

    class VulkanVertexBuffer : public VertexBuffer
    {
        public:
            explicit VulkanVertexBuffer(VulkanRenderer *renderer);
            ~VulkanVertexBuffer() override;
            void Bind() override;
            void UploadData(const std::vector<Vertex> &vertices) override;

        private:
            VulkanRenderer *_renderer;
            std::shared_ptr<VulkanBuffer> _buffer { nullptr };
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
        public:
            explicit VulkanIndexBuffer(VulkanRenderer *renderer);
            ~VulkanIndexBuffer() override;
            void Bind() override;
            void UploadData(const std::vector<uint32_t> &indices) override;
            size_t GetCount() override;

        private:
            VulkanRenderer *_renderer;
            std::shared_ptr<VulkanBuffer> _buffer { nullptr };
            size_t _count { 0 };
    };
}
