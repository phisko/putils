#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace putils::vulkan {
	struct SwapChain {
		vk::UniqueSwapchainKHR swapChain;

		struct Image {
			vk::Image image;
			vk::UniqueImageView view;
			vk::UniqueFramebuffer framebuffer;
			vk::Fence frameUsingMe;
		};

		std::vector<Image> images;
		vk::Format format;
		vk::Extent2D extent;
	};

	struct Context;
	void init(SwapChain & swapChain, const Context & context, const vk::Extent2D & windowSize);
}