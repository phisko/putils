#include "runtime_helper.hpp"

// putils
#include "putils/string.hpp"
#include "putils/profiling.hpp"

namespace putils::reflection::runtime {
	const attribute_info * find_attribute(const type_attributes & attributes, std::string_view path, std::string_view separator) noexcept {
		PUTILS_PROFILING_SCOPE;

		auto * current_attributes = &attributes.map;
		putils::string<1024> current_path(path);

		auto dot_pos = current_path.find(separator);
		while (dot_pos != std::string::npos) {
			const auto nextAttribute = current_path.substr(0, dot_pos);
			current_path = current_path.substr(dot_pos + separator.length());
			dot_pos = current_path.find(separator);

			const auto it = current_attributes->find(nextAttribute.c_str());
			if (it == current_attributes->end()) {
				return nullptr;
			}

			current_attributes = &it->second->attributes.map;
		}

		const auto it = current_attributes->find(current_path.c_str());
		if (it == current_attributes->end()) {
			return nullptr;
		}

		return it->second;
	}
}