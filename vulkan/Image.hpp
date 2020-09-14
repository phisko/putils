#pragma once

#include "vulkan_helper.hpp"
#include "Point.hpp"
#include "ImmediateCommandBuffer.hpp"

namespace putils::vulkan {
	struct Texture {
		vk::UniqueImage image{ nullptr };
		vk::UniqueDeviceMemory memory{ nullptr };
		vk::UniqueImageView view{ nullptr };
	};

	using Vector2ds = ::putils::Vector<vk::DeviceSize, 2>;

	Texture loadTexture(vk::CommandBuffer commandBuffer, vk::Device device, vk::PhysicalDevice physicalDevice, const char * file);
	Texture loadTexture(const putils::vulkan::ImmediateCommandBuffer::Params & params, vk::Device device, vk::PhysicalDevice physicalDevice, const char * file);

	Texture loadTexture(vk::CommandBuffer commandBuffer, vk::Device device, vk::PhysicalDevice physicalDevice, const void * data, size_t width, size_t height, size_t components);
	Texture loadTexture(const putils::vulkan::ImmediateCommandBuffer::Params & params, vk::Device device, vk::PhysicalDevice physicalDevice, const void * data, size_t width, size_t height, size_t components);

	Texture createTexture(vk::Device device, vk::PhysicalDevice physicalDevice, const Vector2ds & size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
	vk::UniqueImageView createImageView(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
	vk::UniqueSampler createTextureSampler(vk::Device device);

	void transitionImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
	void copyBufferToImage(vk::CommandBuffer commandBuffer, vk::Buffer src, vk::Image dst, const Vector2ds & size);
}