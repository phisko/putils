file(GLOB src lua/src/*)
add_library(lua STATIC ${src})
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/lua/src)
