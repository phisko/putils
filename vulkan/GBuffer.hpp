#pragma once

#include <vector>
#include "Image.hpp"

namespace putils::vulkan {
	struct GBuffer {
		vk::UniqueRenderPass renderPass;
		vk::UniqueFramebuffer framebuffer;
		std::vector<putils::vulkan::Texture> textures;

		struct {
			putils::vulkan::Texture texture;
			vk::Format format;
		} depth;
	};
}