#include "ImmediateCommandBuffer.hpp"

namespace putils::vulkan {
	ImmediateCommandBuffer::ImmediateCommandBuffer(const Params & params) : _queue(params.queue) {
		vk::CommandBufferAllocateInfo allocInfo; {
			putils_with(allocInfo);
			_.level = vk::CommandBufferLevel::ePrimary;
			_.commandPool = params.commandPool;
			_.commandBufferCount = 1;
		}

		_commandBuffer = std::move(params.device.allocateCommandBuffersUnique(allocInfo)[0]);

		vk::CommandBufferBeginInfo beginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		_commandBuffer->begin(beginInfo);
	}

	ImmediateCommandBuffer::~ImmediateCommandBuffer() {
		_commandBuffer->end();

		vk::SubmitInfo submitInfo; {
			putils_with(submitInfo);
			_.commandBufferCount = 1;
			_.pCommandBuffers = &*_commandBuffer;
		}
		_queue.submit(submitInfo, nullptr);
		_queue.waitIdle();
	}
}