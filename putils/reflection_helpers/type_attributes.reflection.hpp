#pragma once

#include "putils/reflection.hpp"

#define refltype putils::reflection::runtime::type_attributes
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(map)
	);
	putils_reflection_used_types(
		putils_reflection_type(putils::reflection::runtime::attribute_info)
	);
};
#undef refltype

#define refltype putils::reflection::runtime::attribute_info
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(offset),
		putils_reflection_attribute(size),
		putils_reflection_attribute(type),
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
		putils_reflection_attribute(element_attributes),
		putils_reflection_attribute(get_size_impl),
		putils_reflection_attribute(get_element_impl),
		putils_reflection_attribute(for_each_impl)
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
		putils_reflection_attribute(value_attributes),
		putils_reflection_attribute(get_size_impl),
		putils_reflection_attribute(get_value_impl),
		putils_reflection_attribute(for_each_impl)
	);
	putils_reflection_methods(
		putils_reflection_attribute(get_size),
		putils_reflection_attribute(get_value),
		putils_reflection_attribute(for_each)
	);
};
#undef refltype