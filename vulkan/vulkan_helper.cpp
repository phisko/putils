#include "vulkan_helper.hpp"

namespace putils::vulkan {
	uint32_t findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
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

	void fillMemory(vk::Device device, vk::DeviceMemory dst, const void * src, size_t size) {
		const auto data = device.mapMemory(dst, 0, size);
		memcpy(data, src, size);
		device.unmapMemory(dst);
	}
}
