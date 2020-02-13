#pragma once

#include "pybind11/stl.h"
#include "pybind11/functional.h"
#include "pybind11/chrono.h"
#include "pybind11/cast.h"
#include "pybind11/iostream.h"
#include "pybind11/embed.h"
#include "traits.hpp"
#include "reflection.hpp"
#include "to_string.hpp"
#include "members.hpp"

namespace py = pybind11;

namespace putils {
	namespace python {
		putils_member_detector(begin);
		putils_member_detector(end);

		template<typename T>
		void registerType(py::module & m) {
			py::class_<T> type(m, putils::reflection::get_class_name<T>(), py::dynamic_attr());
			putils::reflection::for_each_attribute<T>([&type](auto name, auto member) {
				using MemberType = std::remove_reference_t<decltype(std::declval<T>().*(member))>;
				if constexpr (std::is_const<MemberType>() || !std::is_assignable<MemberType, MemberType>())
					type.def_readonly(name, member);
				else
					type.def_readwrite(name, member);
			});

			putils::reflection::for_each_method<T>([&type](auto name, auto member) {
				using Ret = putils::member_function_return_type<decltype(member)>;

				if constexpr (std::is_reference<Ret>())
					type.def(name, member, py::return_value_policy::reference);
				else
					type.def(name, member);
			});

            if constexpr (putils::is_streamable<std::ostringstream, T>())
				type.def("__str__", [](const T & obj) { return putils::toString(obj); });

			if constexpr (has_member_begin<T>() && has_member_end<T>())
				type.def("__iter__", [](const T & obj) { return py::make_iterator(obj.begin(), obj.end()); });
        }
    }
}
