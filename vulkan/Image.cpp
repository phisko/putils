#include "Image.hpp"
#include "Buffer.hpp"
#include "vulkan_helper.hpp"
#include "stb_image.h"
#include "with.hpp"
#include "ImmediateCommandBuffer.hpp"

namespace putils::vulkan {
	Texture loadTexture(vk::CommandBuffer commandBuffer, vk::Device device, vk::PhysicalDevice physicalDevice, const char * file) {
		int texWidth, texHeight, texChannels;
		const auto pixels = stbi_load("resources/koala.png", &texWidth, &texHeight, &texChannels, 4);
		if (!pixels)
			return {};

		auto ret = loadTexture(commandBuffer, device, physicalDevice, pixels, (size_t)texWidth, (size_t)texHeight, (size_t)texChannels);
		stbi_image_free(pixels);
		return ret;
	}

	Texture loadTexture(vk::CommandBuffer commandBuffer, vk::Device device, vk::PhysicalDevice physicalDevice, const void * data, size_t width, size_t height, size_t components) {
		const Vector2ds size = { (vk::DeviceSize)width, (vk::DeviceSize)height };

		const auto imageSize = size.x * size.y * components;

		const auto stagingBuffer = createBuffer(
			device,
			physicalDevice,
			imageSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		fillMemory(device, *stagingBuffer.memory, data, (size_t)imageSize);

		const auto format = vk::Format::eR8G8B8A8Srgb;

		auto ret = createTexture(
			device, physicalDevice,
			size,
			format,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		transitionImageLayout(commandBuffer, *ret.image, format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		copyBufferToImage(commandBuffer, *stagingBuffer.buffer, *ret.image, size);
		transitionImageLayout(commandBuffer, *ret.image, format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

		return ret;
	
	}

	Texture loadTexture(const putils::vulkan::ImmediateCommandBuffer::Params & params, vk::Device device, vk::PhysicalDevice physicalDevice, const char * file) {
		int texWidth, texHeight, texChannels;
		const auto pixels = stbi_load("resources/koala.png", &texWidth, &texHeight, &texChannels, 4);
		if (!pixels)
			return {};

		auto ret = loadTexture(params, device, physicalDevice, pixels, (size_t)texWidth, (size_t)texHeight, (size_t)texChannels);
		stbi_image_free(pixels);
		return ret;
	}

	Texture loadTexture(const putils::vulkan::ImmediateCommandBuffer::Params & params, vk::Device device, vk::PhysicalDevice physicalDevice, const void * data, size_t width, size_t height, size_t components) {
		const Vector2ds size = { (vk::DeviceSize)width, (vk::DeviceSize)height };

		const auto imageSize = size.x * size.y * components;

		const auto stagingBuffer = createBuffer(
			device,
			physicalDevice,
			imageSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		fillMemory(device, *stagingBuffer.memory, data, (size_t)imageSize);

		const auto format = vk::Format::eR8G8B8A8Srgb;

		auto ret = createTexture(
			device, physicalDevice,
			size,
			format,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		{
			putils::vulkan::ImmediateCommandBuffer commandBuffer(params);
			transitionImageLayout(*commandBuffer, *ret.image, format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		}

		{
			putils::vulkan::ImmediateCommandBuffer commandBuffer(params);
			copyBufferToImage(*commandBuffer, *stagingBuffer.buffer, *ret.image, size);
		}

		{
			putils::vulkan::ImmediateCommandBuffer commandBuffer(params);
			transitionImageLayout(*commandBuffer, *ret.image, format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
		}

		return ret;
	
	}

	Texture createTexture(vk::Device device, vk::PhysicalDevice physicalDevice, const Vector2ds & size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) {
		Texture ret;

		vk::ImageCreateInfo imageInfo; {
			putils_with(imageInfo);
			_.imageType = vk::ImageType::e2D;
			_.extent.width = (uint32_t)size.x;
			_.extent.height = (uint32_t)size.y;
			_.extent.depth = 1;
			_.mipLevels = 1;
			_.arrayLayers = 1;
			_.format = format;
			_.tiling = tiling;
			_.initialLayout = vk::ImageLayout::eUndefined;
			_.usage = usage;
			_.sharingMode = vk::SharingMode::eExclusive;
			_.samples = vk::SampleCountFlagBits::e1;
		}

		ret.image = device.createImageUnique(imageInfo);

		const auto memRequirements = device.getImageMemoryRequirements(*ret.image);

		vk::MemoryAllocateInfo allocInfo; {
			putils_with(allocInfo);
			_.allocationSize = memRequirements.size;
			_.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
		}

		ret.memory = device.allocateMemoryUnique(allocInfo);
		device.bindImageMemory(*ret.image, *ret.memory, 0);

		const auto aspectFlags =
			usage & vk::ImageUsageFlagBits::eDepthStencilAttachment ?
			vk::ImageAspectFlagBits::eDepth :
			vk::ImageAspectFlagBits::eColor;
		ret.view = createImageView(device, *ret.image, format, aspectFlags);

		return ret;
	}

	vk::UniqueImageView createImageView(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) {
		vk::ImageViewCreateInfo viewInfo; {
			putils_with(viewInfo);
			_.image = image;
			_.viewType = vk::ImageViewType::e2D;
			_.format = format;
			{
				putils_with(_.subresourceRange);
				_.aspectMask = aspectFlags;
				_.baseMipLevel = 0;
				_.levelCount = 1;
				_.baseArrayLayer = 0;
				_.layerCount = 1;
			}
		}

		return device.createImageViewUnique(viewInfo);
	}

	vk::UniqueSampler createTextureSampler(vk::Device device) {
		vk::SamplerCreateInfo samplerInfo; {
			putils_with(samplerInfo);
			_.magFilter = vk::Filter::eLinear;
			_.minFilter = vk::Filter::eLinear;
			_.addressModeU = vk::SamplerAddressMode::eClampToBorder;
			_.addressModeV = vk::SamplerAddressMode::eClampToBorder;
			_.addressModeW = vk::SamplerAddressMode::eClampToBorder;
			_.borderColor = vk::BorderColor::eFloatTransparentBlack;
			_.anisotropyEnable = true;
			_.maxAnisotropy = 16;
			_.unnormalizedCoordinates = false;
			_.compareEnable = false;
			_.compareOp = vk::CompareOp::eAlways;
			_.mipmapMode = vk::SamplerMipmapMode::eLinear;
			_.mipLodBias = 0.f;
			_.minLod = 0.f;
			_.maxLod = 0.f;
		}

		return device.createSamplerUnique(samplerInfo);
	}

	void transitionImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
		vk::PipelineStageFlags sourceStage;
		vk::PipelineStageFlags destinationStage;

		vk::ImageMemoryBarrier barrier; {
			putils_with(barrier);
			_.oldLayout = oldLayout;
			_.newLayout = newLayout;
			_.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			_.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			_.image = image;

			{
				putils_with(_.subresourceRange);
				_.baseMipLevel = 0;
				_.levelCount = 1;
				_.baseArrayLayer = 0;
				_.layerCount = 1;
			}

			_.srcAccessMask = (vk::AccessFlags)0;
			_.dstAccessMask = (vk::AccessFlags)0;
		}

		if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

			if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
				barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
		else
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

		struct LayoutTransition {
			vk::ImageLayout oldLayout;
			vk::ImageLayout newLayout;
		};

		struct Transition {
			struct State {
				vk::ImageLayout layout;
				vk::AccessFlags accessMask;
				vk::PipelineStageFlags stage;
			};

			State src;
			State dst;
		};

		static const struct {
			Transition::State undefined = {
				vk::ImageLayout::eUndefined,
				(vk::AccessFlags)0,
				vk::PipelineStageFlagBits::eTopOfPipe
			};

			Transition::State transferDst = {
				vk::ImageLayout::eTransferDstOptimal,
				vk::AccessFlagBits::eTransferWrite,
				vk::PipelineStageFlagBits::eTransfer
			};

			Transition::State shaderRead = {
				vk::ImageLayout::eShaderReadOnlyOptimal,
				vk::AccessFlagBits::eShaderRead,
				vk::PipelineStageFlagBits::eFragmentShader
			};

			Transition::State depthAttachment = {
				vk::ImageLayout::eDepthStencilAttachmentOptimal,
				vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
				vk::PipelineStageFlagBits::eEarlyFragmentTests
			};
		} states;

		static const Transition transitions[] = {
			{ states.undefined, states.transferDst },
			{ states.transferDst, states.shaderRead },
			{ states.undefined, states.depthAttachment }
		};

		const auto it = std::find_if(std::begin(transitions), std::end(transitions),
			[=](const auto & transition) { return transition.src.layout == oldLayout && transition.dst.layout == newLayout; }
		);
		if (it == std::end(transitions)) {
			assert(false);
			return;
		}

		barrier.srcAccessMask = it->src.accessMask;
		barrier.dstAccessMask = it->dst.accessMask;
		sourceStage = it->src.stage;
		destinationStage = it->dst.stage;

		commandBuffer.pipelineBarrier(
			sourceStage, destinationStage,
			(vk::DependencyFlags)0,
			nullptr, nullptr, barrier
		);
	}

	void copyBufferToImage(vk::CommandBuffer commandBuffer, vk::Buffer src, vk::Image dst, const Vector2ds & size) {
		vk::BufferImageCopy region; {
			putils_with(region);
			_.bufferOffset = 0;
			_.bufferRowLength = 0;
			_.bufferImageHeight = 0;

			{
				putils_with(_.imageSubresource);
				_.aspectMask = vk::ImageAspectFlagBits::eColor;
				_.mipLevel = 0;
				_.baseArrayLayer = 0;
				_.layerCount = 1;
			}

			_.imageOffset = { 0, 0, 0 };
			_.imageExtent = { (uint32_t)size.x, (uint32_t)size.y, 1 };
		}

		commandBuffer.copyBufferToImage(src, dst, vk::ImageLayout::eTransferDstOptimal, region);
	}
}