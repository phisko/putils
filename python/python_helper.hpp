#pragma once

#include "pybind11/pybind11.h"

namespace py = pybind11;

namespace putils::python {
	template<typename T>
	void registerType(py::module_ & m) noexcept;
}

#include "python_helper.inl"
