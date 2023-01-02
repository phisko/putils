// gtest
#include <gtest/gtest.h>

// putils
#include "putils/reflection_helpers/runtime_helper.hpp"

namespace {
	struct reflectible {
		int i = 42;
		std::string s = "hello";

		struct nested {
			std::string s = "hi";
			int i = 84;
		};

		nested nested_object;
		nested nested_array[8];
		std::vector<nested> nested_vector = {
			{ .s = "ho", .i = 21 },
			{ .s = "foo", .i = -42 }
		};
		std::unordered_map<int, nested> nested_map = {
			{ 0, { .s = "bar", .i = 1 } },
			{ 1, { .s = "bar", .i = 2 } }
		};
	};
}

#define refltype reflectible
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(s),
		putils_reflection_attribute(nested_object),
		putils_reflection_attribute(nested_array),
		putils_reflection_attribute(nested_vector),
		putils_reflection_attribute(nested_map));
};
#undef refltype

#define refltype reflectible::nested
putils_reflection_info {
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(s));
};
#undef refltype

TEST(runtime_helper, get_attributes) {
	const auto & attributes = putils::reflection::runtime::get_attributes<reflectible>();
	EXPECT_EQ(attributes.map.size(), 6);
	EXPECT_NE(attributes.map.find("i"), attributes.map.end());
	EXPECT_NE(attributes.map.find("s"), attributes.map.end());
	EXPECT_NE(attributes.map.find("nested_object"), attributes.map.end());
	EXPECT_NE(attributes.map.find("nested_array"), attributes.map.end());
	EXPECT_NE(attributes.map.find("nested_vector"), attributes.map.end());
	EXPECT_NE(attributes.map.find("nested_map"), attributes.map.end());
	EXPECT_EQ(attributes.map.find("foo"), attributes.map.end());
}

TEST(runtime_helper, find_attribute_in_map) {
	const auto & attributes = putils::reflection::runtime::get_attributes<reflectible>();
	EXPECT_NE(putils::reflection::runtime::find_attribute(attributes, "i"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute(attributes, "s"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute(attributes, "nested_object"), nullptr);
	EXPECT_EQ(putils::reflection::runtime::find_attribute(attributes, "foo"), nullptr);
}

TEST(runtime_helper, find_attribute) {
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("i"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("s"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("nested_object"), nullptr);
	EXPECT_EQ(putils::reflection::runtime::find_attribute<reflectible>("foo"), nullptr);
}

TEST(runtime_helper, find_attribute_nested) {
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("nested_object.i"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("nested_object.s"), nullptr);
	EXPECT_EQ(putils::reflection::runtime::find_attribute<reflectible>("nested_object.zob"), nullptr);
}

TEST(runtime_helper, find_attribute_nested_custom_separator) {
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("nested_object/i", "/"), nullptr);
	EXPECT_NE(putils::reflection::runtime::find_attribute<reflectible>("nested_object/s", "/"), nullptr);
	EXPECT_EQ(putils::reflection::runtime::find_attribute<reflectible>("nested_object/zob", "/"), nullptr);
}

TEST(runtime_helper, int_attriubte) {
	const auto attr = putils::reflection::runtime::find_attribute<reflectible>("i");

	EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::i));
	EXPECT_EQ(attr->size, sizeof(int));
	EXPECT_EQ(attr->attributes.map.size(), 0);
	EXPECT_EQ(attr->array, std::nullopt);
	EXPECT_EQ(attr->map, std::nullopt);
}

TEST(runtime_helper, string_attribute) {
	const reflectible obj;
	const auto attr = putils::reflection::runtime::find_attribute<reflectible>("s");

	EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::s));
	EXPECT_EQ(attr->size, sizeof(std::string));
	EXPECT_EQ(attr->attributes.map.size(), 0);

	EXPECT_EQ(attr->array->get_size(&obj.s), obj.s.size());
	for (int i = 0; i < obj.s.size(); ++i)
		EXPECT_EQ(attr->array->get_element(&obj.s, i), &obj.s[i]);

	int i = 0;
	attr->array->for_each(&obj.s, [&](void * element) {
		auto c = (char *)element;
		EXPECT_EQ(c, &obj.s[i]);
		++i;
	});

	EXPECT_EQ(attr->map, std::nullopt);
}

TEST(runtime_helper, reflectible_attribute) {
	const reflectible obj;
	{
		const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_object");
		EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested_object));
		EXPECT_EQ(attr->size, sizeof(reflectible::nested));
		EXPECT_EQ(attr->attributes.map.size(), 2);
		EXPECT_NE(attr->attributes.map.find("i"), attr->attributes.map.end());
		EXPECT_NE(attr->attributes.map.find("s"), attr->attributes.map.end());
		EXPECT_EQ(attr->attributes.map.find("foo"), attr->attributes.map.end());
		EXPECT_EQ(attr->array, std::nullopt);
		EXPECT_EQ(attr->map, std::nullopt);
	}

	{
		const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_object.i");
		EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested::i));
		EXPECT_EQ(attr->size, sizeof(int));
		EXPECT_EQ(attr->attributes.map.size(), 0);
		EXPECT_EQ(attr->array, std::nullopt);
		EXPECT_EQ(attr->map, std::nullopt);
	}

	{
		const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_object.s");

		EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested::s));
		EXPECT_EQ(attr->size, sizeof(std::string));
		EXPECT_EQ(attr->attributes.map.size(), 0);

		EXPECT_EQ(attr->array->get_size(&obj.nested_object.s), obj.nested_object.s.size());
		for (int i = 0; i < obj.nested_object.s.size(); ++i)
			EXPECT_EQ(attr->array->get_element(&obj.nested_object.s, i), &obj.nested_object.s[i]);

		int i = 0;
		attr->array->for_each(&obj.nested_object.s, [&](void * element) {
			auto c = (char *)element;
			EXPECT_EQ(c, &obj.nested_object.s[i]);
			++i;
		});

		EXPECT_EQ(attr->map, std::nullopt);
	}
}

TEST(runtime_helper, array_attribute) {
	const reflectible obj;

	const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_array");
	EXPECT_EQ(attr->size, sizeof(reflectible::nested_array));
	EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested_array));
	EXPECT_EQ(attr->attributes.map.size(), 0);
	EXPECT_EQ(attr->array->get_size(&obj.nested_array), putils::lengthof(obj.nested_array));

	for (int i = 0; i < putils::lengthof(obj.nested_array); ++i)
		EXPECT_EQ(attr->array->get_element(&obj.nested_array, i), &obj.nested_array[i]);

	int i = 0;
	attr->array->for_each(&obj.nested_array, [&](void * element) {
		const auto * nested_object = (reflectible::nested *)element;
		EXPECT_EQ(nested_object->s, obj.nested_array[i].s);
		EXPECT_EQ(nested_object->i, obj.nested_array[i].i);
		++i;
	});

	EXPECT_EQ(attr->array->element_attributes.map.size(), 2);
	EXPECT_NE(attr->array->element_attributes.map.find("i"), attr->array->element_attributes.map.end());
	EXPECT_NE(attr->array->element_attributes.map.find("s"), attr->array->element_attributes.map.end());
	EXPECT_EQ(attr->array->element_attributes.map.find("foo"), attr->array->element_attributes.map.end());

	EXPECT_EQ(attr->map, std::nullopt);
}

TEST(runtime_helper, vector_attribute) {
	const reflectible obj;

	const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_vector");
	EXPECT_EQ(attr->size, sizeof(reflectible::nested_vector));
	EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested_vector));
	EXPECT_EQ(attr->attributes.map.size(), 0);
	EXPECT_EQ(attr->array->get_size(&obj.nested_vector), obj.nested_vector.size());

	for (int i = 0; i < obj.nested_vector.size(); ++i)
		EXPECT_EQ(attr->array->get_element(&obj.nested_vector, i), &obj.nested_vector[i]);

	int i = 0;
	attr->array->for_each(&obj.nested_vector, [&](void * element) {
		const auto * nested_object = (reflectible::nested *)element;
		EXPECT_EQ(nested_object->s, obj.nested_vector[i].s);
		EXPECT_EQ(nested_object->i, obj.nested_vector[i].i);
		++i;
	});

	EXPECT_EQ(attr->array->element_attributes.map.size(), 2);
	EXPECT_NE(attr->array->element_attributes.map.find("i"), attr->array->element_attributes.map.end());
	EXPECT_NE(attr->array->element_attributes.map.find("s"), attr->array->element_attributes.map.end());
	EXPECT_EQ(attr->array->element_attributes.map.find("foo"), attr->array->element_attributes.map.end());

	EXPECT_EQ(attr->map, std::nullopt);
}

TEST(runtime_helper, map_attribute) {
	const reflectible obj;

	const auto attr = putils::reflection::runtime::find_attribute<reflectible>("nested_map");
	EXPECT_EQ(attr->size, sizeof(reflectible::nested_map));
	EXPECT_EQ(attr->offset, putils::member_offset(&reflectible::nested_map));
	EXPECT_EQ(attr->attributes.map.size(), 0);
	EXPECT_EQ(attr->array, std::nullopt);
	EXPECT_EQ(attr->map->get_size(&obj.nested_map), obj.nested_map.size());

	for (const auto & [key, value] : obj.nested_map)
		EXPECT_EQ(attr->map->get_value(&obj.nested_map, putils::to_string(key).c_str()), &value);

	attr->map->for_each(&obj.nested_map, [&](const void * key, void * value) {
		const auto k = (int *)key;
		EXPECT_EQ(&obj.nested_map.at(*k), value);
	});

	EXPECT_EQ(attr->map->key_attributes.map.size(), 0);

	EXPECT_EQ(attr->map->value_attributes.map.size(), 2);
	EXPECT_NE(attr->map->value_attributes.map.find("i"), attr->map->value_attributes.map.end());
	EXPECT_NE(attr->map->value_attributes.map.find("s"), attr->map->value_attributes.map.end());
	EXPECT_EQ(attr->map->value_attributes.map.find("foo"), attr->map->value_attributes.map.end());
}
