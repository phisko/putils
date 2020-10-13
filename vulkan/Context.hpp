#pragma once

#include "Queues.hpp"
#include "SwapChain.hpp"
#include "GBuffer.hpp"

namespace putils::vulkan {
	struct Context {
		vk::UniqueInstance instance;
		vk::PhysicalDevice physicalDevice;
		vk::UniqueDevice device;

		vk::UniqueSurfaceKHR surface;

		vk::UniqueRenderPass renderPass;

		Queues queues;
		SwapChain swapChain;
		GBuffer gbuffer;

		vk::UniqueDescriptorPool descriptorPool;
		vk::UniqueCommandPool commandPool;

		std::vector<vk::UniqueCommandBuffer> commandBuffers;
		std::vector<vk::UniqueCommandBuffer> gbufferCommandBuffers;

		vk::UniqueSampler sampler;

		struct Frame {
			vk::UniqueSemaphore gbufferReady;
			vk::UniqueSemaphore imageAvailable;
			vk::UniqueSemaphore renderFinished;
			vk::UniqueSemaphore gbufferReleased;
			vk::UniqueFence fence;
		};

		std::vector<Frame> frames;
		size_t currentFrame = 0;
		std::optional<size_t> previousFrame = std::nullopt;
	};
}
