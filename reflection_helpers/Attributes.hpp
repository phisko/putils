#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <optional>
#include <memory>

#include "reflection.hpp"

namespace putils::reflection {
	struct AttributeInfo;
	using AttributeMap = std::unordered_map<std::string, AttributeInfo>;

	struct AttributeInfo {
		std::ptrdiff_t offset = 0;
		size_t size = 0;
        putils::meta::type_index type = 0;

		std::unique_ptr<AttributeMap> attributes;
		
		struct ArrayHelper {
			using GetSizeSignature = size_t(const void * attribute);
			GetSizeSignature * getSize = nullptr;

			using GetElementSignature = void *(const void * attribute, size_t index);
			GetElementSignature * getElement = nullptr;
			
			// using Iterator = putils::function<void(void * element), KENGINE_META_ATTRIBUTES_ITERATOR_FUNCTION_SIZE>;
			using Iterator = std::function<void(void * element)>;
			using ForEachSignature = void(const void * attribute, const Iterator & callback);
			ForEachSignature * forEach = nullptr;

			std::unique_ptr<AttributeMap> elementAttributes;
		};
		std::optional<ArrayHelper> arrayHelper;

		struct MapHelper {
			using GetSizeSignature = size_t(const void * attribute);
			GetSizeSignature * getSize = nullptr;

			using GetValueSignature = void *(const void * attribute, const char * keyString);
			GetValueSignature * getValue = nullptr;

			// using Iterator = putils::function<void(const void * key, void * value), KENGINE_META_ATTRIBUTES_ITERATOR_FUNCTION_SIZE>;
			using Iterator = std::function<void(const void * key, void * value)>;
			using ForEachSignature = void(const void * attribute, const Iterator & callback);
			ForEachSignature * forEach = nullptr;

			std::unique_ptr<AttributeMap> keyAttributes;
			std::unique_ptr<AttributeMap> valueAttributes;
		};
		std::optional<MapHelper> mapHelper;
	};

    struct Attributes {
        AttributeMap map;
    };
}

#define refltype putils::reflection::Attributes
putils_reflection_info{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(map)
	);
	putils_reflection_used_types(
		putils_reflection_type(putils::reflection::AttributeInfo)
	);
};
#undef refltype

#define refltype putils::reflection::AttributeInfo
putils_reflection_info{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(offset),
		putils_reflection_attribute(size),
		putils_reflection_attribute(attributes),
		putils_reflection_attribute(arrayHelper),
		putils_reflection_attribute(mapHelper)
	);
	putils_reflection_used_types(
		putils_reflection_type(refltype::ArrayHelper),
		putils_reflection_type(refltype::MapHelper)
	);
};
#undef refltype

#define refltype putils::reflection::AttributeInfo::ArrayHelper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(getSize),
		putils_reflection_attribute(getElement),
		putils_reflection_attribute(forEach),
		putils_reflection_attribute(elementAttributes)
    );
};
#undef refltype

#define refltype putils::reflection::AttributeInfo::MapHelper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(getSize),
		putils_reflection_attribute(getValue),
		putils_reflection_attribute(forEach),
		putils_reflection_attribute(keyAttributes),
		putils_reflection_attribute(valueAttributes)
    );
};
#undef refltype
