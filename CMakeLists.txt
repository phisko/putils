cmake_minimum_required(VERSION 3.0)
project(putils HOMEPAGE_URL "https://github.com/phisko/putils")

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

add_library(putils INTERFACE)

# Core

file(GLOB src *.cpp *.hpp reflection_helpers/*.cpp reflection_helpers/*.hpp)
add_library(putils_core ${src})
target_include_directories(putils_core PUBLIC .)
target_link_libraries(putils INTERFACE putils_core)

option(PUTILS_NDEBUG "Disable debug code" OFF)
if (PUTILS_NDEBUG)
    target_compile_definitions(putils_core PUBLIC PUTILS_NDEBUG)
else()
    if (MSVC)
        target_link_libraries(putils_core PUBLIC Dbghelp)
    endif()
endif()

add_subdirectory(reflection)
target_link_libraries(putils_core PUBLIC reflection)

# Export the symbols if we're built as a DLL
putils_export_symbols(putils_core)

if (UNIX)
    target_compile_options(putils_core PUBLIC -fPIC)
endif()

putils_conan_download_and_link_packages(
    putils_core PUBLIC
    magic_enum/0.7.3
    nlohmann_json/3.10.5
    stb/cci.20210910
    termcolor/2.0.0
)

putils_conan_set_shared_options(options imgui)
putils_conan_download_and_link_packages(
    putils_core PUBLIC
    imgui/cci.20220621+1.88.docking
    IMPORTS # Copy the bindings
        "res, * -> ${CMAKE_CURRENT_BINARY_DIR}/imgui_res"
    ${options}
)

# Profiling

option(PUTILS_PROFILING OFF)
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

option(PUTILS_PLUGIN_MANAGER "Build PluginManager" OFF)
if (PUTILS_PLUGIN_MANAGER)
    add_subdirectory(plugin_manager)
    target_link_libraries(putils INTERFACE putils_plugin_manager)
    target_compile_definitions(putils INTERFACE PUTILS_PLUGIN_MANAGER)
endif()

# Lua

option(PUTILS_LUA "Build Lua helper" OFF)
if(PUTILS_LUA)
    add_subdirectory(lua)
    target_link_libraries(putils INTERFACE putils_lua)
    target_compile_definitions(putils INTERFACE PUTILS_LUA)

endif()

# ImGui Lua bindings

if (PUTILS_LUA)
    file(GLOB imgui_lua_src
            imgui_lua_bindings/*.cpp
            imgui_lua_bindings/*.hpp)
    add_library(putils_imgui_lua_bindings ${imgui_lua_src})
    target_link_libraries(putils_imgui_lua_bindings PUBLIC putils_core)
    target_link_libraries(putils_imgui_lua_bindings)
    target_link_libraries(putils_imgui_lua_bindings PUBLIC putils_lua)
    target_link_libraries(putils INTERFACE putils_imgui_lua_bindings)

    # Export the symbols if we're built as a DLL
    putils_export_symbols(putils_imgui_lua_bindings)

    if (MSVC)
        # Disable "'argument': conversion from 'double' to 'float'" warnings
        target_compile_options(putils_imgui_lua_bindings PRIVATE /wd4244)
    endif()
endif()

# Mediator

option(PUTILS_MEDIATOR "Build Mediator" OFF)
if(PUTILS_MEDIATOR)
    add_subdirectory(mediator)
    target_link_libraries(putils INTERFACE putils_mediator)
    target_compile_definitions(putils INTERFACE PUTILS_MEDIATOR)
endif()

# Python

option(PUTILS_PYTHON "Build Python helper" OFF)
if(PUTILS_PYTHON)
    add_subdirectory(python)
    target_link_libraries(putils INTERFACE putils_python)
    target_compile_definitions(putils INTERFACE PUTILS_PYTHON)
endif()

#
# Tests
#

option(PUTILS_TESTS "Build tests" OFF)
if (PUTILS_TESTS)
	enable_testing()
    set(test_exe_name putils_tests)
    file(GLOB test_src
            tests/*.tests.cpp
            reflection_helpers/tests/*.tests.cpp)

    putils_add_test_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE putils)
endif()
