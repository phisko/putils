#include "python_helper.hpp"

#include "pybind11/stl.h"
#include "pybind11/functional.h"
#include "pybind11/chrono.h"
#include "pybind11/cast.h"
#include "pybind11/iostream.h"
#include "pybind11/embed.h"
#include "reflection.hpp"
#include "to_string.hpp"
#include "meta/traits/is_streamable.hpp"
#include "meta/traits/member_function_return_type.hpp"

namespace putils::python {
	putils_member_detector(begin);
	putils_member_detector(end);

	template<typename T>
	void registerType(py::module_ & m) noexcept {
		try {
			py::class_<T> type(m, putils::reflection::get_class_name<T>(), py::dynamic_attr());
			putils::reflection::for_each_attribute<T>([&type](const auto & attr) noexcept {
				using MemberType = std::remove_reference_t<decltype(std::declval<T>().*(attr.ptr))>;
				if constexpr (std::is_const<MemberType>() || !std::is_assignable<MemberType, MemberType>())
					type.def_readonly(attr.name, attr.ptr);
				else
					type.def_readwrite(attr.name, attr.ptr);
			});

			putils::reflection::for_each_method<T>([&type](const auto & attr) noexcept {
				using Ret = putils::member_function_return_type<decltype(attr.ptr)>;

				if constexpr (std::is_reference<Ret>())
					type.def(attr.name, attr.ptr, py::return_value_policy::reference);
				else
					type.def(attr.name, attr.ptr);
			});

			if constexpr (putils::is_streamable<std::ostringstream, T>())
				type.def("__str__", [](const T & obj) { return putils::toString(obj); });

			if constexpr (has_member_begin<T>() && has_member_end<T>())
				type.def("__iter__", [](const T & obj) { return py::make_iterator(obj.begin(), obj.end()); });
		}
		catch (const std::exception & e) {
			std::cerr << "[Python] Failed to register type: " << e.what() << std::endl;
		}
	}
}
