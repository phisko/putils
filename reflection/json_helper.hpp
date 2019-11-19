#pragma once

#include "json.hpp"
#include "magic_enum.hpp"

namespace putils::reflection {
	template<typename T>
	void fromJSON(const putils::json & object, T && out);

	template<typename T>
	putils::json toJSON(const T & obj);

	namespace detail::json {
		putils_member_detector(c_str);
	}

	template<typename TRef>
	void fromJSON(const putils::json & object, TRef && val) {
		using T = std::remove_reference_t<TRef>;

		if constexpr (detail::json::has_member_c_str<T>::value)
			val = object.get<std::string>().c_str();
		else if constexpr (putils::is_std_vector<T>::value) {
			for (const auto & it : object) {
				val.emplace_back();
				auto & element = val.back();
				fromJSON(it, element);
			}
		}
		else if constexpr (putils::is_vector<T>::value) {
			for (const auto & it : object) {
				auto & element = val.emplace_back();
				fromJSON(it, element);
			}
		}
		else if constexpr (std::is_array<T>::value) {
			size_t i = 0;
			assert(object.array().size() <= lengthof(val));
			for (const auto & it : object) {
				auto & element = val[i];
				fromJSON(it, element);
				++i;
			}
		}
		else if constexpr (putils::reflection::has_attributes<T>::value) {
			putils::reflection::for_each_attribute<T>([&](const char * name, const auto attr) {
				const auto attrJSON = object.find(name);
				if (attrJSON != object.end())
					fromJSON(*attrJSON, val.*attr);
				});
		}
		else if constexpr (std::is_enum<T>::value) {
			for (const auto & p : putils::magic_enum::enum_entries<T>())
				if (object == putils::string<64>(p.second).c_str())
					val = p.first;
		}
		else if constexpr (std::is_scalar<T>::value)
			val = object;
	}

	template<typename T>
	putils::json toJSON(const T & obj) {
		putils::json ret;

		if constexpr (detail::json::has_member_c_str<T>::value)
			ret = obj.c_str();
		else if constexpr (putils::is_iterable<T>::value)
			for (const auto & it : obj)
				ret.push_back(toJSON(it));
		else if constexpr (putils::reflection::has_attributes<T>::value)
			putils::reflection::for_each_attribute<T>([&](const char * name, const auto attr) {
				ret[name] = toJSON(obj.*attr);
			});
		else if constexpr (std::is_enum<T>::value)
			ret = putils::magic_enum::enum_name<T>(obj);
		else if constexpr (std::is_scalar<T>::value)
			ret = obj;

		return ret;
	}
}