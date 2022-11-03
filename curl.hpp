#pragma once

// stl
#include <string>
#include <unordered_map>

namespace putils::curl {
	void downloadFile(const std::string & url, const std::string & outputLocation) noexcept;
	std::string httpRequest(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
	std::string buildCurlCommand(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
}
