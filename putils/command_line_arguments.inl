#include "command_line_arguments.hpp"

// stl
#include <unordered_set>
#include <string>

// reflection
#include "putils/reflection.hpp"

// putils
#include "profiling.hpp"
#include "scn/scn.hpp"
#include "string.hpp"

namespace putils {
	namespace impl {
		template<typename T>
		struct is_optional : std::false_type {};
		template<typename T>
		struct is_optional<std::optional<T>> : std::true_type {};

		template<typename T>
		struct remove_optional;

		template<typename T>
		struct remove_optional<std::optional<T>> {
			using type = T;
		};

		template<typename T>
		struct remove_optional {
			using type = T;
		};

		template<typename T>
		static bool parse_value(std::span<const std::string_view> args, std::string_view key, T & member, bool is_flag) noexcept {
			using attribute_type = impl::remove_optional<putils_typeof(member)>::type;

			const auto parse_value = [&](std::string_view value) {
				if constexpr (impl::is_optional<putils_typeof(member)>())
					(void)scn::scan_default(value, member.emplace());
				else
					(void)scn::scan_default(value, member);
			};

			for (size_t i = 0; i < args.size(); ++i) {
				if (args[i] == key) {
					if constexpr (std::is_same<attribute_type, bool>()) {
						member = true;
						return true;
					}
					++i;
					if (i >= args.size())
						return true;

					parse_value(args[i]);
					return true;
				}
				else if (!is_flag) {
					if (args[i].substr(0, key.size()) != key || args[i][key.size()] != '=')
						continue;
					const auto value = args[i].substr(key.size() + 1);
					parse_value(value);
					return true;
				}
			}
			return false;
		}

		template<typename T>
		static void print_help(std::string_view help) {
			if (!help.empty())
				std::cout << help << std::endl
						  << std::endl;

			if constexpr (putils::reflection::has_class_name<T>())
				std::cout << putils::reflection::get_class_name<T>() << " options:" << std::endl;
			else
				std::cout << "options:" << std::endl;
			putils::reflection::for_each_attribute<T>([](const auto & attr) {
				std::cout << "\t";
				if (const auto flag = putils::reflection::get_metadata<const char *>(attr.metadata, "flag"))
					std::cout << "-" << *flag << ", ";
				std::cout << "--" << attr.name;
				if (const auto desc = putils::reflection::get_metadata<const char *>(attr.metadata, "help"))
					std::cout << "\t\t" << *desc;
				std::cout << std::endl;
			});
			std::cout << std::endl;
		}
	}

	template<typename T>
	T parse_arguments(std::span<const std::string_view> args, std::string_view help) noexcept {
		PUTILS_PROFILING_SCOPE;

		T ret;

		putils::reflection::for_each_attribute(ret, [&](const auto & attr) noexcept {
			const putils::string<1024> option("--{}", attr.name);
			if (impl::parse_value(args, option, attr.member, false))
				return;
			if (const auto flag = putils::reflection::get_metadata<const char *>(attr.metadata, "flag")) {
				const putils::string<1024> dash_flag("-{}", *flag);
				impl::parse_value(args, dash_flag, attr.member, true);
			}
		});

		for (const auto s : args) {
			if (s == "--help") {
				if (!putils::reflection::has_attribute<T>("help")) {
					impl::print_help<T>(help);
					return ret;
				}
			}
			if (s == "-h") {
				bool should_print_help = true;
				putils::reflection::for_each_attribute<T>([&](const auto & attr) noexcept {
					if (const auto flag = putils::reflection::get_metadata<const char *>(attr.metadata, "flag")) {
						if (std::string_view(*flag) == "h")
							should_print_help = false;
					}
				});
				if (should_print_help) {
					impl::print_help<T>(help);
					return ret;
				}
			}
		}

		return ret;
	}
}