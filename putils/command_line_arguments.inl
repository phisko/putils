#include "command_line_arguments.hpp"

// stl
#include <unordered_set>
#include <string>

// reflection
#include "putils/reflection.hpp"

// putils
#include "to_string.hpp"
#include "string.hpp"
#include "profiling.hpp"

namespace putils {
	namespace impl {
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
			using AttributeType = impl::remove_optional<putils_typeof(member)>::type;

			for (size_t i = 0; i < args.size(); ++i) {
				if (args[i] == key) {
					if constexpr (std::is_same<AttributeType, bool>()) {
						member = true;
						return true;
					}
					++i;
					if (i >= args.size())
						return true;
					member = putils::parse<AttributeType>(args[i]);
					return true;
				}
				else if (!is_flag) {
					if (args[i].substr(0, key.size()) != key || args[i][key.size()] != '=')
						continue;
					const auto value = args[i].substr(key.size() + 1);
					member = putils::parse<AttributeType>(value);
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
			const putils::string<1024> option("--%s", attr.name);
			if (impl::parse_value(args, option, attr.member, false))
				return;
			if (const auto flag = putils::reflection::get_metadata<const char *>(attr.metadata, "flag")) {
				const putils::string<1024> dash_flag("-%s", *flag);
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