#pragma once

// sol
#include <sol/sol.hpp>

// meta
#include "meta/traits/is_streamable.hpp"

// putils
#include "function.hpp"
#include "to_string.hpp"
#include "putils_profiling.hpp"

namespace sol {
	namespace detail {
		template <typename Signature, size_t MaxSize>
		struct lua_type_of<putils::function<Signature, MaxSize>> : std::integral_constant<type, type::function> {};
	}

	namespace stack {
		template <typename Signature, size_t MaxSize>
		struct unqualified_pusher<putils::function<Signature, MaxSize>> {
			using TargetFunctor = function_detail::functor_function<putils::function<Signature, MaxSize>, false, true>;

			static int push(lua_State* L, detail::yield_tag_t, const putils::function<Signature, MaxSize>& fx) {
				if (fx) {
					function_detail::select_set_fx<true, false, TargetFunctor>(L, fx);
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, detail::yield_tag_t, putils::function<Signature, MaxSize>&& fx) {
				if (fx) {
					function_detail::select_set_fx<true, false, TargetFunctor>(L, std::move(fx));
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, const putils::function<Signature, MaxSize>& fx) {
				if (fx) {
					function_detail::select_set_fx<false, false, TargetFunctor>(L, fx);
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, putils::function<Signature, MaxSize>&& fx) {
				if (fx) {
					function_detail::select_set_fx<false, false, TargetFunctor>(L, std::move(fx));
					return 1;
				}
				return stack::push(L, lua_nil);
			}
		};
	}

	namespace stack {
		template <typename Signature, size_t MaxSize>
		struct unqualified_getter<putils::function<Signature, MaxSize>> {
			typedef meta::bind_traits<Signature> fx_t;
			typedef typename fx_t::args_list args_lists;
			typedef meta::tuple_types<typename fx_t::return_type> return_types;

			template <typename... R>
			static putils::function<Signature, MaxSize> get_std_func(types<R...>, lua_State* L, int index) {
				detail::std_shim<R...> fx(unsafe_function(L, index));
				return fx;
			}

			static putils::function<Signature, MaxSize> get(lua_State* L, int index, record& tracking) {
				tracking.use(1);
				type t = type_of(L, index);
				if (t == type::none || t == type::lua_nil) {
					return nullptr;
				}
				return get_std_func(return_types(), L, index);
			}
		};
	}
}

namespace putils {
	namespace lua {
		template<typename T>
		void registerType(sol::state & state) {
			PUTILS_PROFILING_SCOPE;

			static_assert(putils::reflection::has_class_name<T>());

			auto type = state.new_usertype<T>(putils::reflection::get_class_name<T>());
			putils::reflection::for_each_attribute<T>([&](const auto & attr) {
				type[attr.name] = attr.ptr;
			});
			putils::reflection::for_each_method<T>([&](const auto & attr) {
				type[attr.name] = attr.ptr;
			});

			if constexpr (putils::is_streamable<std::ostream, T>())
                type[sol::meta_function::to_string] = [](const T & obj) { return putils::toString(obj); };
        }
    }
}