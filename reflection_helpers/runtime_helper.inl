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
				.getSize = [](void * attribute) noexcept {
					auto * array = (MemberType *)attribute;
					if constexpr (std::is_array<MemberType>())
						return putils::lengthof(*array);
					else
						return array->size();
				},
				.getElement = [](void * attribute, size_t index) noexcept -> void * {
					auto * array = (MemberType *)attribute;
					auto & element = (*array)[index];
					return &element;
				},
				.forEach = [](void * attribute, const AttributeInfo::ArrayHelper::Iterator & iterator) noexcept {
					auto * array = (MemberType *)attribute;
					for (auto & element : *array)
						iterator(&element);
				}
			};

			using IndexedType = putils::indexed_type<MemberType>;
			fillAttributes<std::decay_t<IndexedType>>(helper.elementAttributes);

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
			using ValueType = typename MemberType::value_type;

			AttributeInfo::MapHelper helper{
				.getSize = [](void * attribute) noexcept {
					auto * map = (MemberType *)attribute;
					return map->size();
				},
				.getValue = [](void * attribute, const char * keyString) noexcept -> void * {
					auto * map = (MemberType *)attribute;

					const auto key = putils::parse<KeyType>(keyString);

					const auto it = map->find(key);
					if (it != map->end())
						return &*it;
					return nullptr;
				},
				.forEach = [](void * attribute, const AttributeInfo::MapHelper::Iterator & iterator) noexcept {
					auto * map = (MemberType *)attribute;

					for (auto & [key, value] : *map)
						iterator(&key, &value);
				}
			};

			fillAttributes<KeyType>(helper.keyAttributes);
			fillAttributes<ValueType>(helper.valueAttributes);

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
				runtimeAttr.arrayHelper = makeArrayHelper<MemberType>();
				runtimeAttr.mapHelper = makeMapHelper<MemberType>();

				fillAttributes<MemberType>(runtimeAttr.attributes);
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