#pragma once

#include <vulkan/vulkan.hpp>
#include "with.hpp"

namespace putils::vulkan {
	class ImmediateCommandBuffer {
	public:
		struct Params {
			vk::Device device;
			vk::CommandPool commandPool;
			vk::Queue queue;
		};

		ImmediateCommandBuffer(const Params & params);
		~ImmediateCommandBuffer();

		auto operator*() const { return *_commandBuffer; }
		auto & operator->() const { return _commandBuffer; }

	private:
		vk::UniqueCommandBuffer _commandBuffer;
		vk::Queue _queue;
	};
}