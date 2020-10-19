#include "Context.hpp"

#include <unordered_set>

#include "with.hpp"
#include "lengthof.hpp"
#include "vector.hpp"
#include "vulkan_helper.hpp"

namespace putils::vulkan {
	struct ContextImpl {
		static Context create(const CreateContextParams & params) {
			Context context;

			context.instance = createInstance(params);

			if (params.debugMessengerInfo)
				context.debugMessenger = createUniqueDebugUtilsMessengerEXT(*context.instance, *params.debugMessengerInfo);

			context.surface = params.createSurface(*context.instance);
			context.physicalDevice = pickPhysicalDevice(context, params);
			context.device = createLogicalDevice(context, params);

			const auto queueFamilies = findQueueFamilies(context);
			reflection::for_each_attribute(context.queues, [&](const char * name, auto && member) {
				const auto queueFamiliesMember = reflection::get_attribute<QueueFamilyIndices::QueueFamily>(queueFamilies, name);
				assert(queueFamiliesMember != nullptr);
				member = context.device->getQueue(queueFamiliesMember->index.value(), 0);
				});

			context.commandPool = createCommandPool(context);
			init(context.swapChain, context, params.windowSize);

			context.depthFormat = findSupportedFormat(
				context.physicalDevice,
				putils::make_vector(vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint),
				vk::ImageTiling::eOptimal,
				vk::FormatFeatureFlagBits::eDepthStencilAttachment
			);

			for (size_t i = 0; i < context.swapChain.images.size(); ++i)
				context.depthTextures.push_back(createDepth(context));

			context.renderPass = createRenderPass(context);

			context.sampler = createTextureSampler(*context.device);

			context.descriptorPool = createDescriptorPool(context, params);

			createFramebuffers(context);

			return context;
		}

		static vk::UniqueInstance createInstance(const CreateContextParams & params) {
			vk::InstanceCreateInfo instanceInfo; {
				putils_with(instanceInfo);
				_.pApplicationInfo = &params.appInfo;

				_.ppEnabledExtensionNames = params.instanceExtensions.data();
				_.enabledExtensionCount = (uint32_t)params.instanceExtensions.size();

				_.ppEnabledLayerNames = params.layers.data();
				_.enabledLayerCount = (uint32_t)params.layers.size();

				if (params.debugMessengerInfo)
					_.pNext = &*params.debugMessengerInfo;
			}

			return vk::createInstanceUnique(instanceInfo);
		}

		static vk::PhysicalDevice pickPhysicalDevice(const Context & context, const CreateContextParams & params) {
			const auto devices = context.instance->enumeratePhysicalDevices();
			for (const auto & device : devices)
				if (isDeviceSuitable(context, params, device))
					return device;

			assert(false); // No suitable GPU found
			return {};
		}

		static bool isDeviceSuitable(const Context & context, const CreateContextParams & params, const vk::PhysicalDevice & device) {
			if (!putils::vulkan::isComplete(putils::vulkan::findQueueFamilies(context, device)))
				return false;

			if (!checkExtensionSupport(params, device))
				return false;

			const auto formats = device.getSurfaceFormatsKHR(*context.surface);
			if (formats.empty())
				return false;

			const auto presentModes = device.getSurfacePresentModesKHR(*context.surface);
			if (presentModes.empty())
				return false;

			const auto features = device.getFeatures();
			if (!features.samplerAnisotropy)
				return false;

			return true;
		}

		static bool checkExtensionSupport(const CreateContextParams & params, const vk::PhysicalDevice & device) {
			const auto extensions = device.enumerateDeviceExtensionProperties();
			for (const auto required : params.deviceExtensions) {
				const auto it = std::find_if(extensions.begin(), extensions.end(), [&](const vk::ExtensionProperties & extension) {
					return strcmp(extension.extensionName, required) == 0;
					});
				if (it == extensions.end())
					return false;
			}
			return true;
		}

		static vk::UniqueDevice createLogicalDevice(const Context & context, const CreateContextParams & params) {
			const auto queueFamilies = putils::vulkan::findQueueFamilies(context);

			std::unordered_set<uint32_t> uniqueQueueFamilies;
			putils::reflection::for_each_attribute(queueFamilies, [&](const char * name, auto && member) {
				uniqueQueueFamilies.insert(*member.index);
				});

			std::vector<vk::DeviceQueueCreateInfo> queueInfos;
			for (const auto index : uniqueQueueFamilies) {
				vk::DeviceQueueCreateInfo queueInfo; {
					putils_with(queueInfo);
					_.queueFamilyIndex = *queueFamilies.graphics.index;
					_.queueCount = 1;
					static constexpr float queuePriority = 1.f;
					_.pQueuePriorities = &queuePriority;
				}
				queueInfos.push_back(queueInfo);
			}

			vk::PhysicalDeviceFeatures deviceFeatures;
			deviceFeatures.samplerAnisotropy = true;

			vk::DeviceCreateInfo createInfo; {
				putils_with(createInfo);
				_.pQueueCreateInfos = queueInfos.data();
				_.queueCreateInfoCount = (uint32_t)queueInfos.size();

				_.ppEnabledExtensionNames = params.deviceExtensions.data();
				_.enabledExtensionCount = (uint32_t)params.deviceExtensions.size();

				_.pEnabledFeatures = &deviceFeatures;

				_.ppEnabledLayerNames = params.layers.data();
				_.enabledLayerCount = (uint32_t)params.layers.size();
			}

			return context.physicalDevice.createDeviceUnique(createInfo);
		}

		static vk::UniqueCommandPool createCommandPool(const Context & context) {
			vk::CommandPoolCreateInfo poolInfo;
			poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

			poolInfo.queueFamilyIndex = *putils::vulkan::findQueueFamilies(context).graphics.index;
			return context.device->createCommandPoolUnique(poolInfo);
		}

		static vk::UniqueRenderPass createRenderPass(const Context & context) {
			putils::vector<vk::AttachmentDescription, 2> attachments;
			const auto makeAttachmentRef = [&] {
				vk::AttachmentReference ret;
				ret.attachment = (uint32_t)attachments.size() - 1;
				ret.layout = attachments.back().finalLayout;
				return ret;
			};

			{ // color attachment
				putils_with(attachments.emplace_back());
				_.format = context.swapChain.format;
				_.samples = vk::SampleCountFlagBits::e1;
				_.loadOp = vk::AttachmentLoadOp::eClear;
				_.storeOp = vk::AttachmentStoreOp::eStore;
				_.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
				_.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
				_.initialLayout = vk::ImageLayout::eUndefined;
				_.finalLayout = vk::ImageLayout::ePresentSrcKHR;
			}

			auto colorAttachmentRef = makeAttachmentRef();
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			{ // depth attachment
				putils_with(attachments.emplace_back());
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
				_.colorAttachmentCount = 1;
				_.pColorAttachments = &colorAttachmentRef;
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

				_.attachmentCount = (uint32_t)attachments.size();
				_.pAttachments = attachments.data();

				_.subpassCount = 1;
				_.pSubpasses = &subpass;

				_.dependencyCount = 1;
				_.pDependencies = &dependency;
			}

			return context.device->createRenderPassUnique(renderPassInfo);
		}

		static vk::UniqueDescriptorPool createDescriptorPool(const Context & context, const CreateContextParams & params) {
			putils::vector<vk::DescriptorPoolSize, 2> poolSizes;

			const auto frameCount = (uint32_t)context.swapChain.images.size();

			uint32_t maxSets = 0;
			{ // uniforms
				putils_with(poolSizes.emplace_back());
				_.type = vk::DescriptorType::eUniformBuffer;
				_.descriptorCount = frameCount * (uint32_t)params.uniformBuffersPerFrame;
				maxSets += _.descriptorCount;
			}

			{ // samplers
				putils_with(poolSizes.emplace_back());
				_.type = vk::DescriptorType::eCombinedImageSampler;
				_.descriptorCount = frameCount * (uint32_t)params.samplersPerFrame;
				maxSets += _.descriptorCount;
			}

			vk::DescriptorPoolCreateInfo poolInfo;
			poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = maxSets;

			return context.device->createDescriptorPoolUnique(poolInfo);
		}

		static void createFramebuffers(Context & context) {
			for (size_t i = 0; i < context.swapChain.images.size(); ++i) {
				auto & image = context.swapChain.images[i];
				auto & depth = context.depthTextures[i];

				const vk::ImageView attachments[] = {
					*image.view, *depth.view
				};

				vk::FramebufferCreateInfo framebufferInfo; {
					putils_with(framebufferInfo);
					_.renderPass = *context.renderPass;
					_.attachmentCount = (uint32_t)putils::lengthof(attachments);
					_.pAttachments = attachments;
					_.width = context.swapChain.extent.width;
					_.height = context.swapChain.extent.height;
					_.layers = 1;
				}

				image.framebuffer = context.device->createFramebufferUnique(framebufferInfo);
			}
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

		static void recreateSwapChain(Context & context, const vk::Extent2D & windowSize) {
			init(context.swapChain, context, windowSize);
			for (size_t i = 0; i < context.swapChain.images.size(); ++i)
				context.depthTextures.push_back(createDepth(context));
			context.renderPass = createRenderPass(context);
			createFramebuffers(context);
		}
	};


	Context createContext(const CreateContextParams & params) {
		return ContextImpl::create(params);
	}

	ImmediateCommandBuffer::Params getCommandParams(const Context & context) {
		ImmediateCommandBuffer::Params params;
		params.device = *context.device;
		params.commandPool = *context.commandPool;
		params.queue = context.queues.graphics;
		return params;
	}

	void recreateSwapChain(Context & context, const vk::Extent2D & windowSize) {
		ContextImpl::recreateSwapChain(context, windowSize);
	}
}