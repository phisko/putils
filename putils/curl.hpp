#pragma once

// stl
#include <string>
#include <unordered_map>

namespace putils::curl {
	PUTILS_CORE_EXPORT void download_file(const std::string & url, const std::string & output_location) noexcept;
	PUTILS_CORE_EXPORT std::string http_request(const std::string & base_url, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
	PUTILS_CORE_EXPORT std::string build_curl_command(const std::string & base_url, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
}
