#include "runtime_helper.hpp"

// stl
#include <map>
#include <unordered_map>

// meta
#include "putils/meta/concepts/indexable.hpp"
#include "putils/meta/traits/indexed_type.hpp"
#include "putils/meta/traits/is_specialization.hpp"

// putils
#include "putils/to_string.hpp"
#include "putils/lengthof.hpp"
#include "putils/vector.hpp"
#include "putils/profiling.hpp"

namespace putils::reflection::runtime {
	namespace impl {
		template<typename MemberType>
		static attribute_info::array_helper make_array_helper_impl() noexcept {
			PUTILS_PROFILING_SCOPE;

			attribute_info::array_helper helper{
				.get_size_impl = [](const void * attribute) noexcept {
					auto * array = (MemberType *)attribute;
					if constexpr (std::is_array<MemberType>())
						return putils::lengthof(*array);
					else
						return array->size();
					// clang-format off
				},
				// clang-format on
				.get_element_impl = [](const void * attribute, size_t index) noexcept -> void * {
					auto * array = (MemberType *)attribute;
					auto & element = (*array)[(int)index];
					return &element;
				},
				.for_each_impl = [](const void * attribute, const attribute_info::array_helper::iterator & iterator) noexcept {
					auto * array = (MemberType *)attribute;
					for (auto & element : *array)
						iterator(&element);
					// clang-format off
				}
				// clang-format on
			};

			using indexed = std::decay_t<putils::indexed_type<MemberType>>;
			if constexpr (putils::reflection::has_attributes<indexed>()) {
				fill_attributes<indexed>(helper.element_attributes);
			}

			return helper;
		}

		template<typename MemberType>
		static std::optional<attribute_info::array_helper> make_array_helper() noexcept {
			PUTILS_PROFILING_SCOPE;

			if constexpr (std::is_array<MemberType>())
				return make_array_helper_impl<MemberType>();

			if constexpr (putils::indexable<MemberType> && requires(const MemberType & m) { m.size(); })
				if constexpr (std::is_reference<putils::indexed_type<MemberType>>()) // Need this in a nested if, as putils::is_indexable has to be checked first and there is no early-exit for if constexpr
					return make_array_helper_impl<MemberType>();

			return std::nullopt;
		}

		template<typename MemberType>
		static attribute_info::map_helper make_map_helper_impl() noexcept {
			PUTILS_PROFILING_SCOPE;

			using key_type = typename MemberType::key_type;
			using value_type = typename MemberType::mapped_type;

			attribute_info::map_helper helper{
				.get_size_impl = [](const void * attribute) noexcept {
					auto * map = (MemberType *)attribute;
					return map->size();
					// clang-format off
				},
				// clang-format on
				.get_value_impl = [](const void * attribute, const char * key_string) noexcept -> void * {
					auto * map = (MemberType *)attribute;

					const auto key = putils::parse<key_type>(key_string);

					const auto it = map->find(key);
					if (it != map->end())
						return &it->second;
					return nullptr;
				},
				.for_each_impl = [](const void * attribute, const attribute_info::map_helper::iterator & iterator) noexcept {
					auto * map = (MemberType *)attribute;

					for (auto & [key, value] : *map)
						iterator(&key, &value);
					// clang-format off
				}
				// clang-format on
			};

			if constexpr (putils::reflection::has_attributes<key_type>()) {
				fill_attributes<key_type>(helper.key_attributes);
			}

			if constexpr (putils::reflection::has_attributes<value_type>()) {
				fill_attributes<value_type>(helper.value_attributes);
			}

			return helper;
		}

		template<typename MemberType>
		static std::optional<attribute_info::map_helper> make_map_helper() noexcept {
			PUTILS_PROFILING_SCOPE;

			if constexpr (putils::is_specialization<MemberType, std::map>() || putils::is_specialization<MemberType, std::unordered_map>())
				return make_map_helper_impl<MemberType>();

			return std::nullopt;
		}

		template<typename T>
		static void fill_attributes(type_attributes & attributes) noexcept {
			PUTILS_PROFILING_SCOPE;

			putils::reflection::for_each_attribute<T>([&](const auto & attr) noexcept {
				using member_type = putils::member_type<decltype(attr.ptr)>;

				const auto runtime_attr = new attribute_info;
				runtime_attr->size = sizeof(member_type);
				runtime_attr->offset = putils::member_offset(attr.ptr);
				runtime_attr->type = putils::meta::type<member_type>::index;
				runtime_attr->map = make_map_helper<member_type>();
				if (runtime_attr->map == std::nullopt) // avoid having array_helper for std::map<int, ...>
					runtime_attr->array = make_array_helper<member_type>();

				if (putils::reflection::has_attributes<member_type>())
					fill_attributes<member_type>(runtime_attr->attributes);

				attributes.map.emplace(std::string(attr.name), runtime_attr);
			});
		}
	}

	template<typename T>
	const type_attributes & get_attributes() noexcept {
		PUTILS_PROFILING_SCOPE;

		static const type_attributes attributes = [] {
			type_attributes ret;
			impl::fill_attributes<T>(ret);
			return ret;
		}();
		return attributes;
	}

	template<typename T>
	const attribute_info * find_attribute(std::string_view path, std::string_view separator) noexcept {
		PUTILS_PROFILING_SCOPE;
		return find_attribute(get_attributes<T>(), path, separator);
	}
}