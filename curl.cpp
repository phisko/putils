#include <assert.h>
#include "curl.hpp"

namespace putils::curl {
	void downloadFile(const std::string & url, const std::string & outputLocation) {
		system((buildCurlCommand(url) + " > " + outputLocation).c_str());
	}

	std::string buildCurlCommand(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params) {
		std::string ret = baseURL;

		if (!params.empty())
			ret += '?';

		bool first = true;
		for (const auto &[k, v] : params) {
			if (!first)
				ret += '&';
			first = false;

			ret += k;
			ret += '=';
			ret += v;
		}

		return "curl -s \"" + ret + '"';
	}

	std::string httpRequest(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params) {
		std::string s;

		const auto pipe = _popen(buildCurlCommand(baseURL, params).c_str(), "r");
		assert(pipe != nullptr);
		char buffer[1024];
		while (fgets(buffer, sizeof(buffer), pipe))
			s += buffer;
		_pclose(pipe);

		return s;
	}
}