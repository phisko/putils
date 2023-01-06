// meta
#include "putils/meta/traits/is_function.hpp"
#include "putils/meta/traits/is_specialization.hpp"

// putils
#include "json_helper.hpp"
#include "putils/to_string.hpp"
#include "putils/string.hpp"
#include "putils/lengthof.hpp"
#include "putils/profiling.hpp"

namespace putils::reflection {
	namespace detail::json {
		template<typename JSONRef, typename TRef>
		void from_to_json(JSONRef && json_object, TRef && obj) noexcept {
			PUTILS_PROFILING_SCOPE;

			using TNoRef = std::remove_reference_t<TRef>;
			using T = std::decay_t<TRef>;

			using json = std::remove_reference_t<JSONRef>;
			constexpr bool serialize = !std::is_const<json>();

			if constexpr (std::is_same<TNoRef, const char *>()) {
				if constexpr (serialize)
					json_object = obj;
			}

			else if constexpr (std::is_array<TNoRef>() && std::is_same<std::remove_extent_t<TNoRef>, char>()) {
				if constexpr (serialize)
					json_object = obj;
				else {
#ifdef _WIN32
#define MY_STRNCPY strncpy_s
#else
#define MY_STRNCPY strncpy
#endif
					MY_STRNCPY(obj, json_object.template get<std::string>().c_str(), putils::lengthof<TNoRef>());
#undef MY_STRNCPY
				}
			}

			else if constexpr (std::is_array<TNoRef>() && std::is_same<std::remove_extent_t<TNoRef>, const char>()) {
				if constexpr (serialize)
					json_object = obj;
			}

			else if constexpr (requires { obj.c_str(); }) {
				if constexpr (serialize)
					json_object = obj.c_str();
				else
					obj = json_object.template get<std::string>().c_str();
			}

			else if constexpr (putils::is_specialization<T, std::map>() || putils::is_specialization<T, std::unordered_map>()) {
				if constexpr (serialize) {
					for (const auto & [key, value] : obj)
						json_object[putils::to_string(key)] = to_json(value);
				}
				else {
					obj.clear();
					for (const auto & [json_key, value] : json_object.items()) {
						using key_type = typename T::key_type;
						auto key = putils::parse<key_type>(json_key.c_str());
						from_json(value, obj[std::move(key)]);
					}
				}
			}

			else if constexpr (std::ranges::range<T> || std::is_array<TNoRef>()) {
				if constexpr (serialize) {
					for (const auto & it : obj)
						json_object.push_back(to_json(it));
				}
				// clang-format off
				else if constexpr (requires { obj.clear(); obj.back(); } && (requires { obj.emplace_back(); } || requires { obj.push_back({}); })) {
					// clang-format on
					obj.clear();
					for (const auto & it : json_object) {
						if constexpr (requires { obj.emplace_back(); })
							obj.emplace_back();
						else
							obj.push_back({});
						auto & element = obj.back();
						from_to_json(it, element);
					}
				}
				else if constexpr (std::is_array<TNoRef>()) {
					size_t i = 0;
					assert(json_object.array().size() <= lengthof(obj));
					for (const auto & it : json_object) {
						auto & element = obj[i];
						from_json(it, element);
						++i;
					}
				}
			}

			else if constexpr (putils::reflection::has_attributes<T>() || putils::reflection::has_parents<T>()) {
				if constexpr (serialize) {
					putils::reflection::for_each_attribute(obj, [&](const auto & attr) noexcept {
						json_object[attr.name] = to_json(attr.member);
					});
				}
				else {
					putils::reflection::for_each_attribute(obj, [&](const auto & attr) noexcept {
						const auto attr_json = json_object.find(attr.name);
						if (attr_json != json_object.end())
							from_json(*attr_json, attr.member);
					});
				}
			}

			else if constexpr (std::is_enum<T>()) {
				if constexpr (serialize)
					json_object = magic_enum::enum_name<T>(obj);
				else {
					for (const auto & p : magic_enum::enum_entries<T>())
						if (json_object == putils::string<64>(p.second).c_str())
							obj = p.first;
				}
			}

			else if constexpr (std::is_arithmetic<T>()) {
				if constexpr (serialize)
					json_object = obj;
				else
					obj = json_object;
			}

			else if constexpr (putils::is_function<T>()) {
				// Do nothing
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
	void from_json(const nlohmann::json & object, TRef && val) noexcept {
		PUTILS_PROFILING_SCOPE;
		detail::json::from_to_json(object, val);
	}

	template<typename T>
	nlohmann::json to_json(const T & obj) noexcept {
		PUTILS_PROFILING_SCOPE;
		nlohmann::json ret;
		detail::json::from_to_json(ret, obj);
		return ret;
	}
}