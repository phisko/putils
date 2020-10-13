#pragma once

#include <vector>
#include "Buffer.hpp"
#include "Context.hpp"

namespace putils::vulkan {
	class Pipeline {
	public:
		Pipeline(const Context & context);

		virtual void init() = 0;
		virtual void run(vk::CommandBuffer commandBuffer, uint32_t imageIndex) = 0;

	protected:
		void createUniforms(std::span<vk::DescriptorSetLayoutBinding> bindings, size_t bufferSize);
		void createDescriptorSets();

	protected:
		const Context & _context;

		struct {
			vk::UniqueDescriptorSetLayout descriptorSetLayout;
			std::vector<putils::vulkan::Buffer> buffers;
			std::vector<vk::DescriptorSet> descriptorSets; // not unique, as they are automatically freed with the pool
		} _uniforms;

		vk::UniquePipelineLayout _layout;
		vk::UniquePipeline _pipeline;
	};

	vk::UniqueShaderModule createShaderModule(const Context & context, const char * fileName);
}