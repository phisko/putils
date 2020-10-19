#include <fstream>
#include "Pipeline.hpp"

namespace putils::vulkan {
	Pipeline::Pipeline(const Context & context)
		: _context(context)
	{}

	void Pipeline::createUniforms(std::span<vk::DescriptorSetLayoutBinding> bindings, size_t bufferSize) {
		vk::DescriptorSetLayoutCreateInfo layoutInfo; {
			layoutInfo.bindingCount = (uint32_t)bindings.size();
			layoutInfo.pBindings = bindings.data();
		}

		_uniforms.descriptorSetLayout = _context.device->createDescriptorSetLayoutUnique(layoutInfo);

		_uniforms.buffers.clear();
		for (size_t i = 0; i < _context.swapChain.images.size(); ++i) {
			_uniforms.buffers.push_back(putils::vulkan::createBuffer(
				*_context.device,
				_context.physicalDevice,
				bufferSize,
				vk::BufferUsageFlagBits::eUniformBuffer,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
			));
		}
	}

	void Pipeline::createDescriptorSets() {
		const auto swapChainImageCount = (uint32_t)_context.swapChain.images.size();

		std::vector<vk::DescriptorSetLayout> layouts(swapChainImageCount, *_uniforms.descriptorSetLayout);

		vk::DescriptorSetAllocateInfo allocInfo; {
			putils_with(allocInfo);
			_.descriptorPool = *_context.descriptorPool;
			_.descriptorSetCount = swapChainImageCount;
			_.pSetLayouts = layouts.data();
		}

		_uniforms.descriptorSets = _context.device->allocateDescriptorSets(allocInfo);
	}

	static std::vector<char> readFile(const char * fileName) {
		std::ifstream file(fileName, std::ios::ate | std::ios::binary); // ate = at the end, lets us get file size

		if (!file)
			return {};

		const auto fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		return buffer;
	}

	vk::UniqueShaderModule createShaderModule(const Context & context, const char * fileName) {
		const auto code = readFile(fileName);
		if (code.empty())
			return {};

		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = code.size();
		createInfo.pCode = (const uint32_t *)code.data();
		return context.device->createShaderModuleUnique(createInfo);
	}
}