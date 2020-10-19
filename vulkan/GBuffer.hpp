#pragma once

#include <vector>
#include "Image.hpp"

namespace putils::vulkan {
	struct GBuffer {
		vk::UniqueRenderPass renderPass;
		vk::UniqueFramebuffer framebuffer;
		std::vector<putils::vulkan::Texture> textures;
		putils::vulkan::Texture depth;
	};

	struct Context;
	GBuffer createGBuffer(const Context & context, size_t attachments);
}