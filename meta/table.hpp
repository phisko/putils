#pragma once

#include <tuple>
#include "fwd.hpp"
#include "type.hpp"

namespace pmeta {
    // See bottom of file for example use
    template<typename Tuple, typename FlatTuple>
    struct table {
        table(Tuple && tuple, FlatTuple && flatTuple)
                : _tuple(FWD(tuple)), _flatTuple(FWD(flatTuple)) {}

        template<typename Key, typename Func>
        void get_value(Key && key, Func && func);

        template<typename Value, typename Func>
        void get_key(Value && value, Func && func);

        const Tuple & getKeyValues() const { return _tuple; }
        const FlatTuple & getFlatKeyValues() const { return _flatTuple; }

		static constexpr auto size = std::tuple_size<Tuple>::value;
		static constexpr auto flat_size = size * 2;

    private:
        Tuple _tuple;
        FlatTuple _flatTuple;
    };

    template<typename ...KeyValues>
    auto make_table(KeyValues && ...keyValues);

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
        void get_value(Key && key, Table && table, Func && func, std::index_sequence<Is...>);

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func>
        void get_value(Key && key, Table && table, Func && func, std::index_sequence<>) {
            throw std::out_of_range("Not found");
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func, std::size_t I, std::size_t ...Is>
        void get_value(Key && key, Table && table, Func && func, std::index_sequence<I, Is...>) {
            const auto & pair = std::get<I>(table);

			using KeyType = pmeta_typeof(std::get<KPos>(pair));
			if constexpr (std::is_same<KeyType, pmeta_typeof(key)>::value)
			{
				if (std::get<KPos>(pair) == key)
				{
					func(std::get<VPos>(pair));
					return;
				}
			}

            get_value<KPos, VPos>(key, table, func, std::index_sequence<Is...>());
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Func, typename ...Pairs>
        void get_value(Key && key, const std::tuple<Pairs...> & table, Func && func) {
            get_value<KPos, VPos>(FWD(key), table, FWD(func), std::index_sequence_for<Pairs...>());
        }
    }

    template<typename Tuple, typename FlatTuple>
    template<typename Key, typename Func>
    void table<Tuple, FlatTuple>::get_value(Key && key, Func && func) {
        detail::get_value<detail::KeyPos, detail::ValuePos>(FWD(key), _tuple, FWD(func));
    }

    template<typename Tuple, typename FlatTuple>
    template<typename Value, typename Func>
    void table<Tuple, FlatTuple>::get_key(Value && value, Func && func) {
        detail::get_value<detail::ValuePos, detail::KeyPos>(FWD(value), _tuple, FWD(func));
    }

    /*
     * make_table implementation
     */

    namespace detail {
        inline auto make_table() { return std::make_tuple(); }

		template<typename Key, typename Value, typename ...Args>
		auto make_table(Key && key, Value && type, Args && ...args) {
			return std::tuple_cat(
				std::make_tuple(
					std::make_pair(FWD(key), FWD(type))
				),
				detail::make_table(FWD(args)...)
			);
		}
	}

    template<typename ...Args>
    auto make_table(Args && ...args) {
        return table<decltype(detail::make_table(args...)), decltype(std::make_tuple(FWD(args)...))>(
                detail::make_table(args...),
                std::make_tuple(FWD(args)...)
        );
    }

    template<typename ...Args>
    auto make_type_map(Args && ...args) { return make_table(FWD(args)...); }
}
