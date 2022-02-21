#include <unordered_set>

#include "SwapChain.hpp"
#include "Context.hpp"
#include "Image.hpp"
#include "Queues.hpp"
#include "with.hpp"

namespace putils::vulkan {
	static vk::SurfaceFormatKHR pickSwapSurfaceFormat(const Context & context) {
		const auto formats = context.physicalDevice.getSurfaceFormatsKHR(*context.surface);
		for (const auto & format : formats)
			if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return format;

		return formats[0];
	}

	static vk::Extent2D pickSwapExtent(const Context & context, vk::Extent2D windowSize) {
		const auto capabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(*context.surface);

		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;

		windowSize.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, windowSize.width));
		windowSize.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, windowSize.height));
		return windowSize;
	}

	static vk::PresentModeKHR pickSwapPresentMode(const Context & context) {
		const auto presentModes = context.physicalDevice.getSurfacePresentModesKHR(*context.surface);
		for (const auto mode : presentModes)
			if (mode == vk::PresentModeKHR::eMailbox)
				return mode;

		return vk::PresentModeKHR::eFifo;
	}

	void init(SwapChain & swapChain, const Context & context, const vk::Extent2D & windowSize) {
		vk::SwapchainCreateInfoKHR createInfo; {
			putils_with(createInfo);
			_.surface = *context.surface;

			const auto capabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(*context.surface);
			_.minImageCount = capabilities.minImageCount + 1;
			if (capabilities.maxImageCount > 0)
				_.minImageCount = std::min(_.minImageCount, capabilities.maxImageCount);

			const auto surfaceFormat = pickSwapSurfaceFormat(context);
			_.imageFormat = surfaceFormat.format;
			_.imageColorSpace = surfaceFormat.colorSpace;
			_.imageExtent = pickSwapExtent(context, windowSize);
			_.imageArrayLayers = 1;
			_.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

			const auto uniqueIndices = [&] {
				const auto indices = findQueueFamilies(context);
				std::unordered_set<uint32_t> set;
				putils::reflection::for_each_attribute(indices, [&](const auto & attr) {
					set.insert(*attr.member.index);
				});
				return std::vector<uint32_t>(set.begin(), set.end());
			}();

			if (uniqueIndices.size() > 1) {
				_.imageSharingMode = vk::SharingMode::eConcurrent;
				_.queueFamilyIndexCount = (uint32_t)uniqueIndices.size();
				_.pQueueFamilyIndices = uniqueIndices.data();
			}

			_.preTransform = capabilities.currentTransform;
			_.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

			_.presentMode = pickSwapPresentMode(context);
			_.clipped = VK_TRUE;

			const auto & oldSwapchain = swapChain.swapChain;
			_.oldSwapchain = oldSwapchain ? *oldSwapchain : nullptr;
		}

		swapChain.swapChain = context.device->createSwapchainKHRUnique(createInfo);

		swapChain.format = createInfo.imageFormat;
		swapChain.extent = createInfo.imageExtent;

		const auto images = context.device->getSwapchainImagesKHR(*swapChain.swapChain);

		swapChain.images.clear();
		for (auto & image : images) {
			swapChain.images.emplace_back();
			swapChain.images.back().image = image;
			swapChain.images.back().view = putils::vulkan::createImageView(*context.device, image, swapChain.format, vk::ImageAspectFlagBits::eColor);
		}
	}
}
