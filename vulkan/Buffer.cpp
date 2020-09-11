#include "Buffer.hpp"
#include "vulkan_helper.hpp"
#include "with.hpp"

namespace putils::vulkan {
	Buffer createBuffer(vk::Device device, vk::PhysicalDevice physicalDevice, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) {
		Buffer ret;

		vk::BufferCreateInfo bufferInfo{}; {
			putils_with(bufferInfo);
			_.size = size;
			_.usage = usage;
			_.sharingMode = vk::SharingMode::eExclusive;
		}

		ret.buffer = device.createBufferUnique(bufferInfo);

		vk::MemoryAllocateInfo allocInfo{}; {
			putils_with(allocInfo);
			const auto memRequirements = device.getBufferMemoryRequirements(*ret.buffer);
			_.allocationSize = memRequirements.size;
			_.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
		}

		ret.memory = device.allocateMemoryUnique(allocInfo);
		device.bindBufferMemory(*ret.buffer, *ret.memory, 0);

		return ret;
	}

	void copyBuffer(vk::CommandBuffer commandBuffer, vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) {
		vk::BufferCopy copyRegion;
		copyRegion.size = size;
		commandBuffer.copyBuffer(src, dst, copyRegion);
	}
}