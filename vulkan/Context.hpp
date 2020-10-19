#pragma once

#include <vector>
#include <optional>
#include <functional>
#include "Queues.hpp"
#include "SwapChain.hpp"
#include "ImmediateCommandBuffer.hpp"
#include "UniqueDebugUtilsMessengerEXT.hpp"
#include "Image.hpp"

namespace putils::vulkan {
	struct Context {
		vk::UniqueInstance instance;
		vk::PhysicalDevice physicalDevice;
		vk::UniqueDevice device;
		UniqueDebugUtilsMessengerEXT debugMessenger;

		vk::UniqueSurfaceKHR surface;

		vk::UniqueRenderPass renderPass;
		vk::Format depthFormat;

		Queues queues;
		SwapChain swapChain;
		std::vector<Texture> depthTextures;

		vk::UniqueDescriptorPool descriptorPool;
		vk::UniqueCommandPool commandPool;

		vk::UniqueSampler sampler;
	};

	struct CreateContextParams {
		vk::ApplicationInfo appInfo;
		std::optional<vk::DebugUtilsMessengerCreateInfoEXT> debugMessengerInfo;
		std::vector<const char *> instanceExtensions;
		std::vector<const char *> deviceExtensions;
		std::vector<const char *> layers;
		std::function<vk::UniqueSurfaceKHR(vk::Instance instance)> createSurface;
		vk::Extent2D windowSize;
		size_t uniformBuffersPerFrame;
		size_t samplersPerFrame;
		size_t maxFramesInFlight;
	};
	Context createContext(const CreateContextParams & params);
	void recreateSwapChain(Context & context, const vk::Extent2D & windowSize);

	ImmediateCommandBuffer::Params getCommandParams(const Context & context);
}
