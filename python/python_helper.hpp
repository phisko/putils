#pragma once

#include "pybind11/pybind11.h"
#include "pybind11_putils_function.hpp"

namespace py = pybind11;

namespace putils::python {
	template<typename T>
	void registerType(py::module_ & m) noexcept;
}

#include "python_helper.inl"
