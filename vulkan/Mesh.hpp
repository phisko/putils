#pragma once

#include "Buffer.hpp"

namespace putils::vulkan {
	struct Mesh {
		Buffer vertexBuffer;
		Buffer indexBuffer;
		size_t nbIndices = 0;
		vk::IndexType indexType = vk::IndexType::eUint32;
	};

	void draw(vk::CommandBuffer commandBuffer, const Mesh & mesh);
}