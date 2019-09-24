#pragma once

#include "sol.hpp"
#include "traits.hpp"
#include "function.hpp"

namespace sol {
	namespace detail {
		template <typename Signature, size_t MaxSize>
		struct lua_type_of<putils::function<Signature, MaxSize>> : std::integral_constant<type, type::function> {};
	}

	namespace stack {
		template <typename Signature, size_t MaxSize>
		struct unqualified_pusher<putils::function<Signature, MaxSize>> {
			static int push(lua_State* L, detail::yield_tag_t, const putils::function<Signature, MaxSize>& fx) {
				if (fx) {
					function_detail::select<true>(L, fx);
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, detail::yield_tag_t, putils::function<Signature, MaxSize>&& fx) {
				if (fx) {
					function_detail::select<true>(L, std::move(fx));
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, const putils::function<Signature, MaxSize>& fx) {
				if (fx) {
					function_detail::select<false>(L, fx);
					return 1;
				}
				return stack::push(L, lua_nil);
			}

			static int push(lua_State* L, putils::function<Signature, MaxSize>&& fx) {
				if (fx) {
					function_detail::select<false>(L, std::move(fx));
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
				return std::move(fx);
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
			static_assert(putils::has_member_get_class_name<T>());

			auto type = state.new_usertype<T>(T::get_class_name());
			if constexpr (putils::has_member_get_attributes<T>())
				putils::for_each_attribute(T::get_attributes(), [&](const char * name, const auto member) {
					type[name] = member;
				});
			if constexpr (putils::has_member_get_methods<T>())
				putils::for_each_attribute(T::get_methods(), [&](const char * name, const auto member) {
					type[name] = member;
				});

			if constexpr (putils::is_streamable<std::ostream, T>::value)
                type[sol::meta_function::to_string] =
                        [](const T & obj) { return putils::toString(obj); };
        }
    }
}