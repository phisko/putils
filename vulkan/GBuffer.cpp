#include "GBuffer.hpp"
#include "Context.hpp"
#include "vector.hpp"

namespace putils::vulkan {
	GBuffer createGBuffer(const Context & context, size_t attachments) {
		struct impl {
			static GBuffer create(const Context & context, size_t attachments) {
				GBuffer gbuffer;

				gbuffer.depth = createDepth(context);

				std::vector<vk::AttachmentDescription> attachmentDescriptions;
				std::vector<vk::AttachmentReference> colorAttachmentRefs;
				const auto makeAttachmentRef = [&] {
					vk::AttachmentReference ret;
					ret.attachment = (uint32_t)attachmentDescriptions.size() - 1;
					ret.layout = attachmentDescriptions.back().finalLayout;
					return ret;
				};

				for (size_t i = 0; i < attachments; ++i) {
					putils_with(attachmentDescriptions.emplace_back());
					_.format = vk::Format::eR8G8B8A8Srgb;
					_.samples = vk::SampleCountFlagBits::e1;
					_.loadOp = vk::AttachmentLoadOp::eClear;
					_.storeOp = vk::AttachmentStoreOp::eStore;
					_.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
					_.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
					_.initialLayout = vk::ImageLayout::eUndefined;
					_.finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
					colorAttachmentRefs.push_back(makeAttachmentRef());
					colorAttachmentRefs.back().layout = vk::ImageLayout::eColorAttachmentOptimal;
				}

				{ // depth attachment
					putils_with(attachmentDescriptions.emplace_back());
					_.format = context.depthFormat;
					_.samples = vk::SampleCountFlagBits::e1;
					_.loadOp = vk::AttachmentLoadOp::eClear;
					_.storeOp = vk::AttachmentStoreOp::eDontCare;
					_.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
					_.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
					_.initialLayout = vk::ImageLayout::eUndefined;
					_.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
				}

				const auto depthAttachmentRef = makeAttachmentRef();

				vk::SubpassDescription subpass; {
					putils_with(subpass);
					_.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
					_.colorAttachmentCount = (uint32_t)colorAttachmentRefs.size();
					_.pColorAttachments = colorAttachmentRefs.data();
					_.pDepthStencilAttachment = &depthAttachmentRef;
				}

				vk::SubpassDependency dependency; {
					putils_with(dependency);
					_.srcSubpass = VK_SUBPASS_EXTERNAL;
					_.dstSubpass = 0;

					_.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
					_.srcAccessMask = (vk::AccessFlags)0;

					_.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
					_.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
				}

				vk::RenderPassCreateInfo renderPassInfo; {
					putils_with(renderPassInfo);

					_.attachmentCount = (uint32_t)attachmentDescriptions.size();
					_.pAttachments = attachmentDescriptions.data();

					_.subpassCount = 1;
					_.pSubpasses = &subpass;

					_.dependencyCount = 1;
					_.pDependencies = &dependency;
				}

				gbuffer.renderPass = context.device->createRenderPassUnique(renderPassInfo);

				std::vector<vk::ImageView> attachmentViews;
				gbuffer.textures.clear();
				for (size_t i = 0; i < attachments; ++i) {
					gbuffer.textures.push_back(
						putils::vulkan::createTexture(
							*context.device, context.physicalDevice,
							{ context.swapChain.extent.width, context.swapChain.extent.height },
							vk::Format::eR8G8B8A8Srgb,
							vk::ImageTiling::eOptimal,
							vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
							vk::MemoryPropertyFlagBits::eDeviceLocal
						)
					);

					attachmentViews.push_back(*gbuffer.textures.back().view);
				}

				attachmentViews.push_back(*gbuffer.depth.view);

				vk::FramebufferCreateInfo framebufferInfo; {
					putils_with(framebufferInfo);
					_.renderPass = *gbuffer.renderPass;
					_.attachmentCount = (uint32_t)attachmentViews.size();
					_.pAttachments = attachmentViews.data();
					_.width = context.swapChain.extent.width;
					_.height = context.swapChain.extent.height;
					_.layers = 1;
				}

				gbuffer.framebuffer = context.device->createFramebufferUnique(framebufferInfo);

				return gbuffer;
			}

			static Texture createDepth(const Context & context) {
				auto depth = putils::vulkan::createTexture(
					*context.device, context.physicalDevice,
					{ context.swapChain.extent.width, context.swapChain.extent.height },
					context.depthFormat,
					vk::ImageTiling::eOptimal,
					vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled,
					vk::MemoryPropertyFlagBits::eDeviceLocal
				);

				putils::vulkan::ImmediateCommandBuffer commandBuffer(getCommandParams(context));
				putils::vulkan::transitionImageLayout(*commandBuffer, *depth.image, context.depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

				return depth;
			}
		};

		return impl::create(context, attachments);
	}
}