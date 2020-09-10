#pragma once

#include <vulkan/vulkan.hpp>

namespace putils::vulkan {
    struct Buffer {
        vk::UniqueBuffer buffer;
        vk::UniqueDeviceMemory memory;
    };

    Buffer createBuffer(vk::Device & device, const vk::PhysicalDevice & physicalDevice, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
    void copyBuffer(vk::Device & device, vk::CommandPool & pool, vk::Queue & queue, const vk::Buffer & src, vk::Buffer & dst, vk::DeviceSize size);

    void fillBuffer(vk::Device & device, const vk::DeviceMemory & dst, const void * src, size_t size);
    template<typename T>
    void fillBuffer(vk::Device & device, const vk::DeviceMemory & dst, const T & src) { fillBuffer(device, dst, &src, sizeof(src)); }
}