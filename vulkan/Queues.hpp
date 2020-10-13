#pragma once

#include <vulkan/vulkan.hpp>
#include "reflection.hpp"

namespace putils::vulkan {
	struct Queues {
		vk::Queue graphics;
		vk::Queue present;
	};
}

#define refltype putils::vulkan::Queues
putils_reflection_info{
	putils_reflection_attributes(
		putils_reflection_attribute(graphics),
		putils_reflection_attribute(present)
	);
};
#undef refltype

namespace putils::vulkan {
	struct QueueFamilyIndices {
		QueueFamilyIndices(vk::SurfaceKHR surface) : _surface(surface) {}

#define PREDICATE_PARAMS vk::PhysicalDevice device, uint32_t index, const vk::QueueFamilyProperties & properties
		struct QueueFamily {
			using Predicate = bool(PREDICATE_PARAMS);
			const std::function<Predicate> condition = nullptr;
			std::optional<uint32_t> index;
		};

		QueueFamily graphics{
			[](PREDICATE_PARAMS) {
				return properties.queueFlags & vk::QueueFlagBits::eGraphics ? true : false;
			}
		};

		QueueFamily present{
			[this](PREDICATE_PARAMS) {
				return device.getSurfaceSupportKHR(index, _surface);
			}
		};
#undef PREDICATE_PARAMS

	private:
		vk::SurfaceKHR _surface;
	};

	struct Context;
	QueueFamilyIndices findQueueFamilies(const Context & context, vk::PhysicalDevice device = nullptr);
	bool isComplete(const QueueFamilyIndices & indices);
}

#define refltype putils::vulkan::QueueFamilyIndices
putils_reflection_info{
	putils_reflection_attributes(
		putils_reflection_attribute(graphics),
		putils_reflection_attribute(present)
	);
};
#undef refltype


