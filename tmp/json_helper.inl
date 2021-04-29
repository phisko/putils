#include "json_helper.hpp"

namespace putils::reflection {
	namespace detail::json {
		putils_member_detector(c_str);
	}

	template<typename TRef>
	void fromJSON(const putils::json & object, TRef && val) noexcept {
		using T = std::remove_reference_t<TRef>;

		if constexpr (detail::json::has_member_c_str<T>())
			val = object.get<std::string>().c_str();
		else if constexpr (putils::is_specialization<T, std::vector>() || putils::is_vector<T>()) {
			val.clear();
			for (const auto & it : object) {
				val.emplace_back();
				auto & element = val.back();
				fromJSON(it, element);
			}
		}
		else if constexpr (std::is_array<T>()) {
			size_t i = 0;
			assert(object.array().size() <= lengthof(val));
			for (const auto & it : object) {
				auto & element = val[i];
				fromJSON(it, element);
				++i;
			}
		}
		else if constexpr (putils::reflection::has_attributes<T>() || putils::reflection::has_parents<T>()) {
			putils::reflection::for_each_attribute(val, [&](const char * name, auto && attr) noexcept {
				const auto attrJSON = object.find(name);
				if (attrJSON != object.end())
					fromJSON(*attrJSON, attr);
			});
		}
		else if constexpr (std::is_enum<T>()) {
			for (const auto & p : putils::magic_enum::enum_entries<T>())
				if (object == putils::string<64>(p.second).c_str())
					val = p.first;
		}
		else if constexpr (std::is_scalar<T>())
			val = object;
	}

	template<typename T>
	putils::json toJSON(const T & obj) noexcept {
		putils::json ret;

		if constexpr (detail::json::has_member_c_str<T>())
			ret = obj.c_str();
		else if constexpr (putils::is_iterable<T>())
			for (const auto & it : obj)
				ret.push_back(toJSON(it));
		else if constexpr (putils::reflection::has_attributes<T>() || putils::reflection::has_parents<T>())
			putils::reflection::for_each_attribute(obj, [&](const char * name, auto && attr) noexcept {
				ret[name] = toJSON(attr);
			});
		else if constexpr (std::is_enum<T>())
			ret = putils::magic_enum::enum_name<T>(obj);
		else if constexpr (std::is_scalar<T>())
			ret = obj;

		return ret;
	}
}