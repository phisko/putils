add_library(putils_lua INTERFACE)
target_include_directories(putils_lua INTERFACE .)

find_package(Lua REQUIRED)
target_include_directories(putils_lua INTERFACE ${LUA_INCLUDE_DIR})
target_link_libraries(putils_lua INTERFACE ${LUA_LIBRARIES})

find_package(sol2 CONFIG REQUIRED)
target_link_libraries(putils_lua INTERFACE sol2::sol2)