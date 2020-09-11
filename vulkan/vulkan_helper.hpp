#pragma once

#include <vulkan/vulkan.hpp>
#include "with.hpp"

namespace putils::vulkan {
    uint32_t findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

    void fillMemory(vk::Device device, vk::DeviceMemory dst, const void * src, size_t size);
    template<typename T>
    void fillMemory(vk::Device device, vk::DeviceMemory dst, const T & src) { fillMemory(device, dst, &src, sizeof(src)); }

	struct VertexDescription {
		vk::VertexInputBindingDescription binding;
		std::vector<vk::VertexInputAttributeDescription> attributes;
	};

	template<typename T>
	VertexDescription getVertexDescription();
}

// Impl
namespace putils::vulkan {
	template<typename T>
	VertexDescription getVertexDescription() {
		static_assert(putils::reflection::has_attributes<T>());

		VertexDescription desc;

		desc.binding.binding = 0;
		desc.binding.stride = sizeof(T);
		desc.binding.inputRate = vk::VertexInputRate::eVertex;

		uint32_t location = 0;
		putils::reflection::for_each_attribute<T>([&](const char * name, const auto member) {
			vk::VertexInputAttributeDescription attribute;
			attribute.binding = 0;
			attribute.location = location++;
			attribute.offset = (uint32_t)putils::member_offset(member);

			using Member = putils::MemberType<putils_typeof(member)>;

			const auto setFormat = [&](const vk::Format(&formats)[4]) {
				constexpr auto length = putils::lengthof<Member>();
				attribute.format = formats[length - 1];
			};

			using ArraySubType = std::remove_all_extents_t<Member>;
#define IsArrayOf(T) std::is_same_v<ArraySubType, T>
			if constexpr (IsArrayOf(float)) {
				setFormat({
					vk::Format::eR32Sfloat,
					vk::Format::eR32G32Sfloat,
					vk::Format::eR32G32B32Sfloat,
					vk::Format::eR32G32B32A32Sfloat
					});
			}
			else if constexpr (IsArrayOf(int)) {
				setFormat({
					vk::Format::eR32Sint,
					vk::Format::eR32G32Sint,
					vk::Format::eR32G32B32Sint,
					vk::Format::eR32G32B32A32Sint
					});
			}
			else if constexpr (IsArrayOf(unsigned int)) {
				setFormat({
					vk::Format::eR32Uint,
					vk::Format::eR32G32Uint,
					vk::Format::eR32G32B32Uint,
					vk::Format::eR32G32B32A32Uint
					});
			}
			else {
				static_assert(false);
			}
#undef IsArrayOf

			desc.attributes.push_back(attribute);
			});

		return desc;
	}
}