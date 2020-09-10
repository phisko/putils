#include "Buffer.hpp"
#include "with.hpp"

namespace putils::vulkan {
#pragma region createBuffer
#pragma region declarations
	static uint32_t findMemoryType(const vk::PhysicalDevice & physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
#pragma endregion
	Buffer createBuffer(vk::Device & device, const vk::PhysicalDevice & physicalDevice, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) {
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

	static uint32_t findMemoryType(const vk::PhysicalDevice & physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
		const auto memProperties = physicalDevice.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
			const bool suitableType = typeFilter & (1 << i);
			if (!suitableType)
				continue;
			const bool hasProperties = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
			if (!hasProperties)
				continue;
			return i;
		}

		assert(false);
		return ~0;
	}
#pragma endregion createBuffer

	void copyBuffer(vk::Device & device, vk::CommandPool & pool, vk::Queue & queue, const vk::Buffer & src, vk::Buffer & dst, vk::DeviceSize size) {
		vk::CommandBufferAllocateInfo allocInfo;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandPool = pool;
		allocInfo.commandBufferCount = 1;

		const auto commandBuffers = device.allocateCommandBuffersUnique(allocInfo);
		const auto & commandBuffer = commandBuffers[0];

		vk::CommandBufferBeginInfo beginInfo; {
			beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		}

		commandBuffer->begin(beginInfo);

		vk::BufferCopy copyRegion; {
			copyRegion.size = size;
		}

		commandBuffer->copyBuffer(src, dst, copyRegion);

		commandBuffer->end();

		vk::SubmitInfo submitInfo; {
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &*commandBuffer;
		}

		queue.submit(submitInfo, nullptr);
		queue.waitIdle();
	}

	void fillBuffer(vk::Device & device, const vk::DeviceMemory & dst, const void * src, size_t size) {
		const auto data = device.mapMemory(dst, 0, size);
		memcpy(data, src, size);
		device.unmapMemory(dst);
	}
}