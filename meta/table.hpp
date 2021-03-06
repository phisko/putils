#pragma once

#include <cassert>
#include <tuple>
#include "fwd.hpp"
#include "type.hpp"

namespace putils {
    template<typename Tuple>
    using table = Tuple;

    template<typename ...KeyValues>
    constexpr auto make_table(KeyValues && ...keyValues) noexcept;

    /*
     * Implementation details
     */

    namespace detail {
        enum Indexes {
            KeyPos = 0,
            ValuePos = 1
        };
    }

    /*
     * get_value implementation
     */

    namespace detail {
        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func, std::size_t ...Is>
        constexpr void get_value(Key && key, Table && table, Func && func, std::index_sequence<Is...>) noexcept;

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func>
        constexpr void get_value(Key && key, Table && table, Func && func, std::index_sequence<>) noexcept {
			assert(!"Key not found");
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func, std::size_t I, std::size_t ...Is>
        constexpr void get_value(Key && key, Table && table, Func && func, std::index_sequence<I, Is...>) noexcept {
            const auto & pair = std::get<I>(table);

			using KeyType = putils_typeof(std::get<KPos>(pair));
			if constexpr (std::is_same<KeyType, putils_typeof(key)>::value) {
				if (std::get<KPos>(pair) == key) {
					func(std::get<VPos>(pair));
					return;
				}
			}

            get_value<KPos, VPos>(key, table, func, std::index_sequence<Is...>());
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Func, typename ...Pairs>
        constexpr void get_value(Key && key, const std::tuple<Pairs...> & table, Func && func) noexcept {
            get_value<KPos, VPos>(FWD(key), table, FWD(func), std::index_sequence_for<Pairs...>());
        }
    }

    template<typename Table, typename Key, typename Func>
    constexpr void get_value(Table && table, Key && key, Func && func) noexcept {
        detail::get_value<detail::KeyPos, detail::ValuePos>(FWD(key), table, FWD(func));
    }

    template<typename Table, typename Value, typename Func>
    constexpr void get_key(Table && table, Value && value, Func && func) noexcept {
        detail::get_value<detail::ValuePos, detail::KeyPos>(FWD(value), table, FWD(func));
    }

    /*
     * make_table implementation
     */

    namespace detail {
        inline constexpr auto make_table() noexcept { return std::make_tuple(); }

		template<typename Key, typename Value, typename ...Args>
		constexpr auto make_table(Key && key, Value && type, Args && ...args) noexcept {
			return std::tuple_cat(
				std::make_tuple(
					std::make_pair(FWD(key), FWD(type))
				),
				detail::make_table(FWD(args)...)
			);
		}
	}

    template<typename ...Args>
    constexpr auto make_table(Args && ...args) noexcept {
        return detail::make_table(args...);
    }

    template<typename ...Args>
    constexpr auto make_type_map(Args && ...args) noexcept { return make_table(FWD(args)...); }
}
