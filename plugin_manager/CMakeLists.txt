file(GLOB src *.cpp *.hpp *.inl)
add_library(putils_plugin_manager ${src})
target_include_directories(putils_plugin_manager PUBLIC .)
target_link_libraries(putils_plugin_manager PUBLIC putils_core)
if(UNIX)
	target_link_libraries(putils_plugin_manager PUBLIC dl)
endif()

# Export the symbols if we're built as a DLL
putils_export_symbols(putils_plugin_manager)