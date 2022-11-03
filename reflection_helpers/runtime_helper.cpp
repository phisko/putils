#include "runtime_helper.hpp"

// putils
#include "string.hpp"
#include "putils_profiling.hpp"

namespace putils::reflection::runtime {
	const AttributeInfo * findAttribute(const Attributes & attributes, std::string_view path, std::string_view separator) noexcept {
		PUTILS_PROFILING_SCOPE;

		auto * currentAttributes = &attributes.map;
		putils::string<1024> currentPath(path);

		auto dotPos = currentPath.find(separator);
		while (dotPos != std::string::npos) {
			const auto nextAttribute = currentPath.substr(0, dotPos);
			currentPath = currentPath.substr(dotPos + separator.length());
			dotPos = currentPath.find(separator);

			const auto it = currentAttributes->find(nextAttribute.c_str());
			if (it == currentAttributes->end()) {
				return nullptr;
			}

			currentAttributes = &it->second->attributes.map;
		}

		const auto it = currentAttributes->find(currentPath.c_str());
		if (it == currentAttributes->end()) {
			return nullptr;
		}

		return it->second;
	}
}