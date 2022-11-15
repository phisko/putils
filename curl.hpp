#pragma once

// stl
#include <string>
#include <unordered_map>

namespace putils::curl {
	PUTILS_CORE_EXPORT void downloadFile(const std::string & url, const std::string & outputLocation) noexcept;
	PUTILS_CORE_EXPORT std::string httpRequest(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
	PUTILS_CORE_EXPORT std::string buildCurlCommand(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
}
