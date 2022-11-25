#pragma once

// stl
#include <unordered_map>
#include <string>
#include <functional>
#include <optional>
#include <memory>

// reflection
#include "putils/reflection.hpp"

namespace putils::reflection::runtime {
	struct attribute_info;

    struct PUTILS_CORE_EXPORT type_attributes {
        std::unordered_map<std::string, const attribute_info *> map;
    };

	struct PUTILS_CORE_EXPORT attribute_info {
		std::ptrdiff_t offset = 0;
		size_t size = 0;
        putils::meta::type_index type = 0;

		type_attributes attributes;
		
		struct PUTILS_CORE_EXPORT array_helper {
            size_t get_size(const void * attribute) const noexcept {
                return get_size_impl(attribute);
            }

            void * get_element(const void * attribute, size_t index) const noexcept {
                return get_element_impl(attribute, index);
            }

            using iterator = std::function<void(void * element)>;
            void for_each(const void * attribute, const iterator & callback) const noexcept {
                return for_each_impl(attribute, callback);
            }

            type_attributes element_attributes;

        //private:
            using get_size_signature = putils::member_function_signature<decltype(&array_helper::get_size)>;
            get_size_signature * get_size_impl = nullptr;

            using get_element_signature = putils::member_function_signature<decltype(&array_helper::get_element)>;
            get_element_signature * get_element_impl = nullptr;

            using for_each_signature = putils::member_function_signature<decltype(&array_helper::for_each)>;
            for_each_signature * for_each_impl = nullptr;
		};

		std::optional<array_helper> array;

		struct PUTILS_CORE_EXPORT map_helper {
            size_t get_size(const void * attribute) const noexcept {
                return get_size_impl(attribute);
            }

            void * get_value(const void * attribute, const char * key_string) const noexcept {
                return get_value_impl(attribute, key_string);
            }

            using iterator = std::function<void(const void * key, void * value)>;
            void for_each(const void * attribute, const iterator & callback) const noexcept {
                return for_each_impl(attribute, callback);
            }

            type_attributes key_attributes;
            type_attributes value_attributes;

        //private:
            using get_size_signature = putils::member_function_signature<decltype(&map_helper::get_size)>;
			get_size_signature * get_size_impl = nullptr;

			using get_value_signature = void *(const void * attribute, const char * key_string);
			get_value_signature * get_value_impl = nullptr;

			using for_each_signature = void(const void * attribute, const iterator & callback);
			for_each_signature * for_each_impl = nullptr;
		};
		std::optional<map_helper> map;
	};
}

#define refltype putils::reflection::runtime::type_attributes
putils_reflection_info{
	putils_reflection_custom_class_name(putils_attributes);
	putils_reflection_attributes(
		putils_reflection_attribute(map)
	);
	putils_reflection_used_types(
		putils_reflection_type(putils::reflection::runtime::attribute_info)
	);
};
#undef refltype

#define refltype putils::reflection::runtime::attribute_info
putils_reflection_info{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(offset),
		putils_reflection_attribute(size),
		putils_reflection_attribute(attributes),
		putils_reflection_attribute(array),
		putils_reflection_attribute(map)
	);
	putils_reflection_used_types(
		putils_reflection_type(refltype::array_helper),
		putils_reflection_type(refltype::map_helper)
	);
};
#undef refltype

#define refltype putils::reflection::runtime::attribute_info::array_helper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(element_attributes)
    );
    putils_reflection_methods(
        putils_reflection_attribute(get_size),
        putils_reflection_attribute(get_element),
        putils_reflection_attribute(for_each)
    );
};
#undef refltype

#define refltype putils::reflection::runtime::attribute_info::map_helper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(key_attributes),
		putils_reflection_attribute(value_attributes)
    );
    putils_reflection_methods(
        putils_reflection_attribute(get_size),
        putils_reflection_attribute(get_value),
        putils_reflection_attribute(for_each)
    );
};
#undef refltype
