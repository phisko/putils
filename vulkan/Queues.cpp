#include "Queues.hpp"
#include "Context.hpp"

namespace putils::vulkan {
	QueueFamilyIndices findQueueFamilies(const Context & context, vk::PhysicalDevice device) {
		if (!device)
			device = context.physicalDevice;

		QueueFamilyIndices indices(*context.surface);

		uint32_t i = 0;
		for (const auto & queueFamily : device.getQueueFamilyProperties()) {
			putils::reflection::for_each_attribute(indices, [&](const char * name, auto && member) {
				if (member.condition(device, i, queueFamily))
					member.index = i;
				});
			if (isComplete(indices))
				break;
			++i;
		}

		return indices;
	}

	bool isComplete(const QueueFamilyIndices & indices) {
		bool good = true;
		putils::reflection::for_each_attribute(indices, [&](const char * name, auto && member) {
			good &= member.index.has_value();
			});
		return good;
	}


}