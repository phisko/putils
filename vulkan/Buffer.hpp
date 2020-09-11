#pragma once

#include "vulkan_helper.hpp"

namespace putils::vulkan {
    struct Buffer {
        vk::UniqueBuffer buffer{ nullptr };
        vk::UniqueDeviceMemory memory{ nullptr };
    };

    Buffer createBuffer(vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
    void copyBuffer(vk::CommandBuffer commandBuffer, vk::Buffer src, vk::Buffer dst, vk::DeviceSize size);
}