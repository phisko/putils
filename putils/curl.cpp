#include "curl.hpp"

// stl
#include <cassert>

// putils
#include "curl.hpp"
#include "putils/profiling.hpp"

namespace putils::curl {
	void download_file(const std::string & url, const std::string & output_location) noexcept {
		PUTILS_PROFILING_SCOPE;
		system((build_curl_command(url) + " > " + output_location).c_str());
	}

	std::string build_curl_command(const std::string & base_url, const std::unordered_map<std::string, std::string> & params) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::string ret = base_url;

		if (!params.empty())
			ret += '?';

		bool first = true;
		for (const auto & [k, v] : params) {
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

	std::string http_request(const std::string & base_url, const std::unordered_map<std::string, std::string> & params) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::string s;

		const auto pipe = MY_POPEN(build_curl_command(base_url, params).c_str(), "r");
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