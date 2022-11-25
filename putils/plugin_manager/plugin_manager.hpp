#pragma once

// stl
#include <string>
#include <unordered_map>

// putils
#include "library.hpp"
#include "putils/vector.hpp"

namespace putils {
	class plugin_manager {
	public:
		// In each plugin, execute the [name] function, taking P as parameter
		template<typename ... P>
		void execute(const std::string & name, P &&... params) noexcept;

		// In each plugin, execute the [name] function, returning a T and taking a P as parameter
		// Returns a vector of all values returned
		template<size_t MaxReturns, typename T, typename ... P>
		putils::vector<T, MaxReturns> execute_with_return(const std::string & name, P && ... params) noexcept;

		// Same as execute, but only in libraries which have been added to the directory since we last scanned
		template<typename ... P>
		void rescan_directory(const char * path, const char * to_execute, P && ... params) noexcept;

		template<size_t MaxReturns, typename T, typename ... P>
		putils::vector<T, MaxReturns> rescan_directory_with_return(const char * path, const char * to_execute, P && ... params) noexcept;

	private:
		std::unordered_map<std::string, library *> _libraries;
	};
}

#include "plugin_manager.inl"
