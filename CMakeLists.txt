cmake_minimum_required(VERSION 3.0)
project(putils HOMEPAGE_URL "https://github.com/phisko/putils")

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

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

putils_conan_download_and_link_packages(
    putils_core PUBLIC
    magic_enum/0.7.3
    nlohmann_json/3.10.5
)

putils_conan_set_shared_options(options imgui)
putils_conan_download_and_link_packages(
    putils_core PUBLIC
    imgui/cci.20220621+1.88.docking
    IMPORTS # Copy the bindings
        "res, * -> ${CMAKE_CURRENT_BINARY_DIR}/imgui_res"
    ${options}
)

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
    file(GLOB test_src
            putils/tests/*.tests.cpp
            putils/reflection_helpers/tests/*.tests.cpp)

    putils_add_test_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE putils)
endif()
