#include "runtime_helper.hpp"

#include <map>
#include <unordered_map>

#include "to_string.hpp"
#include "lengthof.hpp"
#include "vector.hpp"
#include "meta/traits/is_indexable.hpp"
#include "meta/traits/indexed_type.hpp"
#include "meta/traits/is_specialization.hpp"

namespace putils::reflection::runtime {
    namespace impl {
		putils_member_detector(size);

		template<typename MemberType>
		static AttributeInfo::ArrayHelper makeArrayHelperImpl() noexcept {
			AttributeInfo::ArrayHelper helper{
				.getSize = [](const void * attribute) noexcept {
					auto * array = (MemberType *)attribute;
					if constexpr (std::is_array<MemberType>())
						return putils::lengthof(*array);
					else
						return array->size();
				},
				.getElement = [](const void * attribute, size_t index) noexcept -> void * {
					auto * array = (MemberType *)attribute;
					auto & element = (*array)[(int)index];
					return &element;
				},
				.forEach = [](const void * attribute, const AttributeInfo::ArrayHelper::Iterator & iterator) noexcept {
					auto * array = (MemberType *)attribute;
					for (auto & element : *array)
						iterator(&element);
				}
			};

			using IndexedType = std::decay_t<putils::indexed_type<MemberType>>;
            if constexpr (putils::reflection::has_attributes<IndexedType>()) {
                helper.elementAttributes = std::make_unique<AttributeMap>();
                fillAttributes<IndexedType>(*helper.elementAttributes);
            }

			return helper;
		}

    	template<typename MemberType>
    	static std::optional<AttributeInfo::ArrayHelper> makeArrayHelper() noexcept {
			if constexpr (std::is_array<MemberType>())
				return makeArrayHelperImpl<MemberType>();

			if constexpr (putils::is_indexable<MemberType>() && has_member_size<MemberType>())
				if constexpr (std::is_reference<putils::indexed_type<MemberType>>()) // Need this in a nested if, as putils::is_indexable has to be checked first and there is no early-exit for if constexpr
					return makeArrayHelperImpl<MemberType>();

    		return std::nullopt;
    	}

		template<typename MemberType>
		static AttributeInfo::MapHelper makeMapHelperImpl() noexcept {
			using KeyType = typename MemberType::key_type;
			using ValueType = typename MemberType::mapped_type;

			AttributeInfo::MapHelper helper{
				.getSize = [](const void * attribute) noexcept {
					auto * map = (MemberType *)attribute;
					return map->size();
				},
				.getValue = [](const void * attribute, const char * keyString) noexcept -> void * {
					auto * map = (MemberType *)attribute;

					const auto key = putils::parse<KeyType>(keyString);

					const auto it = map->find(key);
					if (it != map->end())
						return &it->second;
					return nullptr;
				},
				.forEach = [](const void * attribute, const AttributeInfo::MapHelper::Iterator & iterator) noexcept {
					auto * map = (MemberType *)attribute;

					for (auto & [key, value] : *map)
						iterator(&key, &value);
				}
			};

            if constexpr (putils::reflection::has_attributes<KeyType>()) {
                helper.keyAttributes = std::make_unique<AttributeMap>();
                fillAttributes<KeyType>(*helper.keyAttributes);
            }

            if constexpr (putils::reflection::has_attributes<ValueType>()) {
                helper.valueAttributes = std::make_unique<AttributeMap>();
                fillAttributes<ValueType>(*helper.valueAttributes);
            }

			return helper;
		}

    	template<typename MemberType>
    	static std::optional<AttributeInfo::MapHelper> makeMapHelper() noexcept {
			if constexpr (putils::is_specialization<MemberType, std::map>() || putils::is_specialization<MemberType, std::unordered_map>())
				return makeMapHelperImpl<MemberType>();

			return std::nullopt;
		}

		template<typename T>
		static void fillAttributes(AttributeMap & attributes) noexcept {
			putils::reflection::for_each_attribute<T>([&](const auto & attr) noexcept {
				using MemberType = putils::MemberType<decltype(attr.ptr)>;

				auto & runtimeAttr = attributes[std::string(attr.name)];
				runtimeAttr.size = sizeof(MemberType);
				runtimeAttr.offset = putils::member_offset(attr.ptr);
                runtimeAttr.type = putils::meta::type<MemberType>::index;
				runtimeAttr.mapHelper = makeMapHelper<MemberType>();
                if (runtimeAttr.mapHelper == std::nullopt) // avoid having arrayHelper for std::map<int, ...>
                    runtimeAttr.arrayHelper = makeArrayHelper<MemberType>();

                if (putils::reflection::has_attributes<MemberType>()) {
                    runtimeAttr.attributes = std::make_unique<AttributeMap>();
                    fillAttributes<MemberType>(*runtimeAttr.attributes);
                }
			});
		}
        
    }

    template<typename T>
    const Attributes & getAttributes() noexcept {
        static const Attributes attributes = [] {
            Attributes ret;
            impl::fillAttributes<T>(ret.map);
            return ret;
        }();
        return attributes;
    }

	template<typename T>
	const AttributeInfo * findAttribute(std::string_view path, std::string_view separator) noexcept {
	    return findAttribute(getAttributes<T>(), path, separator);
    }
}