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

		const Vector2ds size = { (vk::DeviceSize)texWidth, (vk::DeviceSize)texHeight };

		const auto imageSize = size.x * size.y * 4;

		const auto stagingBuffer = createBuffer(
			device,
			physicalDevice,
			imageSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		fillMemory(device, *stagingBuffer.memory, pixels, (size_t)imageSize);

		stbi_image_free(pixels);

		const auto format = vk::Format::eR8G8B8A8Srgb;

		auto ret = createImage(
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

		const Vector2ds size = { (vk::DeviceSize)texWidth, (vk::DeviceSize)texHeight };

		const auto imageSize = size.x * size.y * 4;

		const auto stagingBuffer = createBuffer(
			device,
			physicalDevice,
			imageSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		fillMemory(device, *stagingBuffer.memory, pixels, (size_t)imageSize);

		stbi_image_free(pixels);

		const auto format = vk::Format::eR8G8B8A8Srgb;

		auto ret = createImage(
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


	Texture createImage(vk::Device device, vk::PhysicalDevice physicalDevice, const Vector2ds & size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) {
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

		ret.view = createImageView(device, *ret.image, format);

		return ret;
	}

	vk::UniqueImageView createImageView(vk::Device device, vk::Image image, vk::Format format) {
		vk::ImageViewCreateInfo viewInfo; {
			putils_with(viewInfo);
			_.image = image;
			_.viewType = vk::ImageViewType::e2D;
			_.format = format;
			{
				putils_with(_.subresourceRange);
				_.aspectMask = vk::ImageAspectFlagBits::eColor;
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
				_.aspectMask = vk::ImageAspectFlagBits::eColor;
				_.baseMipLevel = 0;
				_.levelCount = 1;
				_.baseArrayLayer = 0;
				_.layerCount = 1;
			}

			_.srcAccessMask = (vk::AccessFlags)0;
			_.dstAccessMask = (vk::AccessFlags)0;
		}

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
			barrier.srcAccessMask = (vk::AccessFlags)0;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}

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