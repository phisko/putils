cmake_minimum_required(VERSION 3.0)
project(putils HOMEPAGE_URL "https://github.com/phisko/putils")

set(CMAKE_CXX_STANDARD 20)

add_library(putils INTERFACE)

add_subdirectory(reflection)

# Core

putils_src_files(
    src
    DIRECTORIES
        putils
        putils/reflection_helpers
    EXTENSIONS
        cpp hpp
)
add_library(putils_core ${src})
target_include_directories(putils_core PUBLIC .)
target_link_libraries(putils_core PUBLIC putils_reflection)

target_link_libraries(putils INTERFACE putils_core)

option(PUTILS_NDEBUG "Disable debug code")
if (PUTILS_NDEBUG)
    target_compile_definitions(putils_core PUBLIC PUTILS_NDEBUG)
else()
    if (MSVC)
        target_link_libraries(putils_core PUBLIC Dbghelp)
    endif()
endif()

# Export the symbols if we're built as a DLL
putils_export_symbols(putils_core)

if (UNIX)
    target_compile_options(putils_core PUBLIC -fPIC)
endif()

if(MSVC)
    target_compile_options(putils_core INTERFACE /DNOMINMAX)
endif()

find_package(fmt CONFIG REQUIRED)
target_link_libraries(putils_core PUBLIC fmt::fmt)

find_package(scn CONFIG REQUIRED)
target_link_libraries(putils_core PUBLIC scn::scn)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(putils_core PUBLIC magic_enum::magic_enum)

option(PUTILS_GENERATE_REFLECTION "Generate reflection headers")
if(PUTILS_GENERATE_REFLECTION)
    putils_generate_reflection_headers(
            TARGET putils_core
            SOURCES ${src}
    )
endif()

# Profiling

option(PUTILS_PROFILING "Enable profiling with Tracy")
if(PUTILS_PROFILING)
	target_compile_definitions(putils_core PUBLIC PUTILS_PROFILING TRACY_ENABLE)
    if (MSVC)
        target_compile_options(putils_core PUBLIC /wd4101) # disable warnings about unreferenced tracy scopes
    endif()

	include(FetchContent)
	FetchContent_Declare(
		tracy
		GIT_REPOSITORY https://github.com/wolfpld/tracy
		GIT_TAG v0.9
		GIT_PROGRESS TRUE
		GIT_SHALLOW TRUE
	)
	FetchContent_MakeAvailable(tracy)
    if(MSVC)
        target_compile_options(TracyClient PUBLIC /DNOMINMAX)
    endif()
	target_link_libraries(putils_core PUBLIC TracyClient)
endif()

# Plugin manager

option(PUTILS_PLUGIN_MANAGER "Build plugin_manager")
if (PUTILS_PLUGIN_MANAGER)
    add_subdirectory(putils/plugin_manager)
    target_link_libraries(putils INTERFACE putils_plugin_manager)
    target_compile_definitions(putils INTERFACE PUTILS_PLUGIN_MANAGER)
endif()

# Lua
option(PUTILS_LUA "Build Lua helper")
if(PUTILS_LUA)
    add_subdirectory(putils/lua)
    target_link_libraries(putils INTERFACE putils_lua)
    target_compile_definitions(putils INTERFACE PUTILS_LUA)
endif()

# Python
option(PUTILS_PYTHON "Build Python helper")
if(PUTILS_PYTHON)
    add_subdirectory(putils/python)
    target_link_libraries(putils INTERFACE putils_python)
    target_compile_definitions(putils INTERFACE PUTILS_PYTHON)
endif()

#
# Tests
#

option(PUTILS_TESTS "Build tests")
if (PUTILS_TESTS)
	enable_testing()
    set(test_exe_name putils_tests)
    file(GLOB_RECURSE test_src putils/*.tests.cpp)

    putils_add_test_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE putils)

    find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(${test_exe_name} PRIVATE nlohmann_json::nlohmann_json)
endif()
