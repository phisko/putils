file(GLOB src lua/src/*)
add_library(putils_lua STATIC ${src})
target_include_directories(putils_lua PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/lua/src)
