#pragma once

#include <vulkan/vulkan.hpp>

namespace putils::vulkan {
	struct UniqueDebugUtilsMessengerEXT {
		vk::Instance instance;
		vk::DispatchLoaderDynamic dispatchLoader;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

		UniqueDebugUtilsMessengerEXT() noexcept = default;
		~UniqueDebugUtilsMessengerEXT() noexcept;
		UniqueDebugUtilsMessengerEXT(UniqueDebugUtilsMessengerEXT && rhs) noexcept;
		UniqueDebugUtilsMessengerEXT & operator=(UniqueDebugUtilsMessengerEXT && rhs) noexcept;
	};

	UniqueDebugUtilsMessengerEXT createUniqueDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerCreateInfoEXT createInfo);
}