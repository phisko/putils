// meta
#include "meta/traits/is_iterable.hpp"
#include "meta/traits/is_specialization.hpp"

// putils
#include "json_helper.hpp"
#include "to_string.hpp"
#include "string.hpp"
#include "lengthof.hpp"
#include "putils_profiling.hpp"

namespace putils::reflection {
	namespace detail::json {
		putils_member_detector(c_str);
		putils_member_detector(emplace_back);
        putils_member_detector(push_back);

		template<typename JSONRef, typename TRef>
		void fromToJSON(JSONRef && jsonObject, TRef && obj) noexcept {
			PUTILS_PROFILING_SCOPE;

            using TNoRef = std::remove_reference_t<TRef>;
			using T = std::decay_t<TRef>;

			using JSON = std::remove_reference_t<JSONRef>;
			constexpr bool serialize = !std::is_const<JSON>();

            if constexpr (std::is_same<TNoRef, const char *>()) {
                if constexpr (serialize)
                    jsonObject = obj;
            }

            else if constexpr (std::is_array<TNoRef>() && std::is_same<std::remove_extent_t<TNoRef>, char>()) {
                if constexpr (serialize)
                    jsonObject = obj;
                else {
#ifdef _WIN32
# define MY_STRNCPY strncpy_s
#else
# define MY_STRNCPY strncpy
#endif
                    MY_STRNCPY(obj, jsonObject.template get<std::string>().c_str(), putils::lengthof<TNoRef>());
#undef MY_STRNCPY
                }
            }

            else if constexpr (std::is_array<TNoRef>() && std::is_same<std::remove_extent_t<TNoRef>, const char>()) {
                if constexpr (serialize)
                    jsonObject = obj;
            }

			else if constexpr (detail::json::has_member_c_str<T>()) {
				if constexpr (serialize)
					jsonObject = obj.c_str();
				else
					obj = jsonObject.template get<std::string>().c_str();
			}

			else if constexpr (putils::is_specialization<T, std::map>() || putils::is_specialization<T, std::unordered_map>()) {
				if constexpr (serialize) {
					for (const auto& [key, value] : obj)
						jsonObject[putils::toString(key)] = toJSON(value);
				}
				else {
					obj.clear();
					for (const auto& [jsonKey, value] : jsonObject.items()) {
						using DesiredKey = typename T::key_type;
						auto key = putils::parse<DesiredKey>(jsonKey.c_str());
						fromJSON(value, obj[std::move(key)]);
					}
				}
			}

			else if constexpr (putils::is_iterable<T>() || std::is_array<TNoRef>()) {
				if constexpr (serialize) {
					for (const auto & it : obj)
						jsonObject.push_back(toJSON(it));
				}
				else if constexpr (putils::is_specialization<T, std::vector>() || detail::json::has_member_emplace_back<T>() || detail::json::has_member_push_back<T>()) {
					obj.clear();
					for (const auto & it : jsonObject) {
                        if constexpr (detail::json::has_member_emplace_back<T>())
                            obj.emplace_back();
                        else
                            obj.push_back({});
						auto & element = obj.back();
						fromToJSON(it, element);
					}
				}
				else if constexpr (std::is_array<TNoRef>()) {
					size_t i = 0;
					assert(jsonObject.array().size() <= lengthof(obj));
					for (const auto & it : jsonObject) {
						auto & element = obj[i];
						fromJSON(it, element);
						++i;
					}
				}
			}

			else if constexpr (putils::reflection::has_attributes<T>() || putils::reflection::has_parents<T>()) {
				if constexpr (serialize) {
					putils::reflection::for_each_attribute(obj, [&](const auto & attr) noexcept {
						jsonObject[attr.name] = toJSON(attr.member);
					});
				}
				else {
					putils::reflection::for_each_attribute(obj, [&](const auto & attr) noexcept {
						const auto attrJSON = jsonObject.find(attr.name);
						if (attrJSON != jsonObject.end())
							fromJSON(*attrJSON, attr.member);
					});
				}
			}
			
			else if constexpr (std::is_enum<T>()) {
				if constexpr (serialize)
					jsonObject = magic_enum::enum_name<T>(obj);
				else {
					for (const auto & p : magic_enum::enum_entries<T>())
						if (jsonObject == putils::string<64>(p.second).c_str())
							obj = p.first;
				}
			}

			else if constexpr (std::is_arithmetic<T>()) {
				if constexpr (serialize)
					jsonObject = obj;
				else
					obj = jsonObject;
			}

            else if constexpr (std::is_empty<T>()) {
                // Do nothing
            }

			else {
				assert(false);
			}
		}
	}

	template<typename TRef>
	void fromJSON(const nlohmann::json & object, TRef && val) noexcept {
		PUTILS_PROFILING_SCOPE;
		detail::json::fromToJSON(object, val);
	}

	template<typename T>
	nlohmann::json toJSON(const T & obj) noexcept {
		PUTILS_PROFILING_SCOPE;
		nlohmann::json ret;
		detail::json::fromToJSON(ret, obj);
		return ret;
	}
}