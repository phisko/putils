#include <assert.h>
#include "curl.hpp"

namespace putils::curl {
	void downloadFile(const std::string & url, const std::string & outputLocation) noexcept {
		system((buildCurlCommand(url) + " > " + outputLocation).c_str());
	}

	std::string buildCurlCommand(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params) noexcept {
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

#ifdef _WIN32
# define MY_POPEN _popen
# define MY_PCLOSE _pclose
#else
# define MY_POPEN popen
# define MY_PCLOSE pclose
#endif

	std::string httpRequest(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params) noexcept {
		std::string s;

		const auto pipe = MY_POPEN(buildCurlCommand(baseURL, params).c_str(), "r");
		assert(pipe != nullptr);
		char buffer[1024];
		while (fgets(buffer, sizeof(buffer), pipe))
			s += buffer;
		MY_PCLOSE(pipe);

		return s;
	}

#undef MY_POPEN
#undef MY_PCLOSE
}