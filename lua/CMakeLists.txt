file(GLOB src lua/src/*)
add_library(putils_lua STATIC ${src})
target_include_directories(putils_lua PUBLIC . lua/src)