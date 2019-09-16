add_library(connection INTERFACE)
target_include_directories(connection INTERFACE . include)
target_link_libraries(connection INTERFACE ssl crypto)
