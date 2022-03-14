#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <optional>
#include <memory>

#include "reflection.hpp"

namespace putils::reflection::runtime {
	struct AttributeInfo;

    struct Attributes {
        std::unordered_map<std::string, const AttributeInfo *> map;
    };

	struct AttributeInfo {
		std::ptrdiff_t offset = 0;
		size_t size = 0;
        putils::meta::type_index type = 0;

		Attributes attributes;
		
		struct ArrayHelper {
            size_t getSize(const void * attribute) const noexcept {
                return getSizeImpl(attribute);
            }

            void * getElement(const void * attribute, size_t index) const noexcept {
                return getElementImpl(attribute, index);
            }

            using Iterator = std::function<void(void * element)>;
            void forEach(const void * attribute, const Iterator & callback) const noexcept {
                return forEachImpl(attribute, callback);
            }

            Attributes elementAttributes;

        //private:
            using GetSizeSignature = putils::member_function_signature<decltype(&ArrayHelper::getSize)>;
            GetSizeSignature * getSizeImpl = nullptr;

            using GetElementSignature = putils::member_function_signature<decltype(&ArrayHelper::getElement)>;
            GetElementSignature * getElementImpl = nullptr;

            using ForEachSignature = putils::member_function_signature<decltype(&ArrayHelper::forEach)>;
            ForEachSignature * forEachImpl = nullptr;
		};

		std::optional<ArrayHelper> arrayHelper;

		struct MapHelper {
            size_t getSize(const void * attribute) const noexcept {
                return getSizeImpl(attribute);
            }

            void * getValue(const void * attribute, const char * keyString) const noexcept {
                return getValueImpl(attribute, keyString);
            }

            using Iterator = std::function<void(const void * key, void * value)>;
            void forEach(const void * attribute, const Iterator & callback) const noexcept {
                return forEachImpl(attribute, callback);
            }

            Attributes keyAttributes;
            Attributes valueAttributes;

        //private:
            using GetSizeSignature = putils::member_function_signature<decltype(&MapHelper::getSize)>;
			GetSizeSignature * getSizeImpl = nullptr;

			using GetValueSignature = void *(const void * attribute, const char * keyString);
			GetValueSignature * getValueImpl = nullptr;

			using ForEachSignature = void(const void * attribute, const Iterator & callback);
			ForEachSignature * forEachImpl = nullptr;
		};
		std::optional<MapHelper> mapHelper;
	};
}

#define refltype putils::reflection::runtime::Attributes
putils_reflection_info{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(map)
	);
	putils_reflection_used_types(
		putils_reflection_type(putils::reflection::runtime::AttributeInfo)
	);
};
#undef refltype

#define refltype putils::reflection::runtime::AttributeInfo
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

#define refltype putils::reflection::runtime::AttributeInfo::ArrayHelper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(elementAttributes)
    );
    putils_reflection_methods(
        putils_reflection_attribute(getSize),
        putils_reflection_attribute(getElement),
        putils_reflection_attribute(forEach)
    );
};
#undef refltype

#define refltype putils::reflection::runtime::AttributeInfo::MapHelper
putils_reflection_info {
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(keyAttributes),
		putils_reflection_attribute(valueAttributes)
    );
    putils_reflection_methods(
        putils_reflection_attribute(getSize),
        putils_reflection_attribute(getValue),
        putils_reflection_attribute(forEach)
    );
};
#undef refltype
