#pragma once

#include <span>
#include <vulkan/vulkan.hpp>
#include "with.hpp"
#include "traits.hpp"
#include "members.hpp"
#include "meta/type.hpp"

namespace putils::vulkan {
    uint32_t findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
	vk::Format findSupportedFormat(vk::PhysicalDevice physicalDevice, std::span<vk::Format> candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

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
	namespace detail {
		putils_nested_type_detector(DataType);
	}

	template<typename MemberPtr>
	vk::VertexInputAttributeDescription getVertexAttributeDescription(uint32_t location, MemberPtr member, uint32_t offset = 0) {
		vk::VertexInputAttributeDescription attribute;

		attribute.binding = 0;
		attribute.location = location;
		attribute.offset = offset + (uint32_t)putils::member_offset(member);

		using Member = putils::MemberType<MemberPtr>;

		const auto setFormat = [&](const vk::Format(&formats)[4]) {
			constexpr auto length = sizeof(Member) / sizeof(std::remove_all_extents_t<Member>);
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

		return attribute;
	}

	template<typename T>
	VertexDescription getVertexDescription() {
		VertexDescription desc;

		desc.binding.binding = 0;
		desc.binding.stride = sizeof(T);
		desc.binding.inputRate = vk::VertexInputRate::eVertex;

		static constexpr bool isPolyVoxType = putils::vulkan::detail::has_nested_type_DataType<T>{};

		uint32_t location = 0;
		if constexpr (isPolyVoxType) {
			vk::VertexInputAttributeDescription attr;
			attr.binding = 0;
			attr.location = location++;
			attr.format = vk::Format::eR32G32B32Sfloat;
			attr.offset = offsetof(T, position);
			desc.attributes.push_back(attr);

			attr.location = location++;
			attr.offset = offsetof(T, normal);
			desc.attributes.push_back(attr);

			using Data = typename T::DataType;
			const auto dataOffset = offsetof(T, data);
			putils::reflection::for_each_attribute<Data>([&](const char * name, const auto member) {
				desc.attributes.push_back(getVertexAttributeDescription(location++, member, (uint32_t)dataOffset));
			});
		}
		else {
			putils::reflection::for_each_attribute<T>([&](const char * name, const auto member) {
				desc.attributes.push_back(getVertexAttributeDescription(location++, member));
			});
		}

		return desc;
	}
}