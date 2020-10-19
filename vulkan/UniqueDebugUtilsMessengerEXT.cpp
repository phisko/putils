#include "UniqueDebugUtilsMessengerEXT.hpp"

namespace putils::vulkan {
	UniqueDebugUtilsMessengerEXT createUniqueDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerCreateInfoEXT createInfo) {
		UniqueDebugUtilsMessengerEXT messenger;
		messenger.instance = instance;
		messenger.dispatchLoader.init(instance, vkGetInstanceProcAddr);
		const auto ret = messenger.dispatchLoader.vkCreateDebugUtilsMessengerEXT(instance, (VkDebugUtilsMessengerCreateInfoEXT *)&createInfo, nullptr, &messenger.debugMessenger);
		assert(ret == VK_SUCCESS);
		return messenger;
	}

	UniqueDebugUtilsMessengerEXT::~UniqueDebugUtilsMessengerEXT() noexcept {
		if (debugMessenger != VK_NULL_HANDLE)
			dispatchLoader.vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	UniqueDebugUtilsMessengerEXT::UniqueDebugUtilsMessengerEXT(UniqueDebugUtilsMessengerEXT && rhs) noexcept {
		*this = std::move(rhs);
	}

	UniqueDebugUtilsMessengerEXT & UniqueDebugUtilsMessengerEXT::operator=(UniqueDebugUtilsMessengerEXT && rhs) noexcept {
		using std::swap;
		swap(instance, rhs.instance);
		swap(dispatchLoader, rhs.dispatchLoader);
		swap(debugMessenger, rhs.debugMessenger);
		return *this;
	}
}