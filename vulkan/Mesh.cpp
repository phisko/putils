#include "Mesh.hpp"

namespace putils::vulkan {
	void draw(vk::CommandBuffer commandBuffer, const Mesh & mesh) {
		commandBuffer.bindVertexBuffers(0, *mesh.vertexBuffer.buffer, (uint64_t)0);
		commandBuffer.bindIndexBuffer(*mesh.indexBuffer.buffer, 0, vk::IndexType::eUint16);
		commandBuffer.drawIndexed((uint32_t)mesh.nbIndices, 1, 0, 0, 0);
	}
}