#include "python_helper.hpp"

// pybind11
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <pybind11/cast.h>
#include <pybind11/iostream.h>
#include <pybind11/embed.h>

// meta
#include "putils/meta/concepts/streamable.hpp"
#include "putils/meta/traits/member_function_return_type.hpp"

// reflection
#include "putils/reflection.hpp"

// putils
#include "putils/profiling.hpp"
#include "putils/range.hpp"
#include "putils/to_string.hpp"

namespace putils::python {
	template<typename T>
	void register_type(py::module_ & m) noexcept {
		PUTILS_PROFILING_SCOPE;

		try {
			const py::gil_scoped_acquire acquire; // In case we're called from a worker thread

			py::class_<T> type(m, putils::reflection::get_class_name<T>(), py::dynamic_attr());
			putils::reflection::for_each_attribute<T>([&type](const auto & attr) noexcept {
				using member_type = std::remove_reference_t<decltype(std::declval<T>().*(attr.ptr))>;
				if constexpr (std::is_const<member_type>() || !std::is_assignable<member_type, member_type>())
					type.def_readonly(attr.name, attr.ptr);
				else
					type.def_readwrite(attr.name, attr.ptr);
			});

			putils::reflection::for_each_method<T>([&type](const auto & attr) noexcept {
				using ret = putils::member_function_return_type<decltype(attr.ptr)>;

				if constexpr (std::is_reference<ret>())
					type.def(attr.name, attr.ptr, py::return_value_policy::reference);
				else
					type.def(attr.name, attr.ptr);
			});

			if constexpr (putils::streamable<T, std::stringstream>)
				type.def("__str__", [](const T & obj) { return putils::to_string(obj); });

			if constexpr (std::ranges::range<T>)
				type.def("__iter__", [](const T & obj) { return py::make_iterator(putils_range(obj)); });
		}
		catch (const std::exception & e) {
			std::cerr << "[Python] Failed to register type: " << e.what() << std::endl;
		}
	}
}
