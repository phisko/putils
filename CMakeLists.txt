set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/CMakeModules)
include(cmake_helper)
include(conan_helper)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

file(GLOB src *.cpp *.hpp)
add_library(putils STATIC ${src})
target_include_directories(putils PUBLIC .)

add_subdirectory(pluginManager)
target_link_libraries(putils PUBLIC pluginManager)

set(imguiSrc imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_draw.cpp)
add_library(imgui STATIC ${imguiSrc})
target_include_directories(imgui PUBLIC imgui)
target_link_libraries(putils PUBLIC imgui)

if(PUTILS_BUILD_LUA)
    add_subdirectory(lua)
    target_link_libraries(putils PUBLIC lua)

    file(GLOB imgui_lua_src
        imgui_lua_bindings/*.cpp
        imgui_lua_bindings/*.hpp)
    add_library(imgui_lua_bindings STATIC ${imgui_lua_src})
    target_link_libraries(imgui_lua_bindings PUBLIC putils)
    target_link_libraries(putils PUBLIC imgui_lua_bindings)
endif()

if(PUTILS_BUILD_MEDIATOR)
    add_subdirectory(mediator)
    target_link_libraries(putils PUBLIC mediator)
endif()

if(PUTILS_BUILD_PYTHON)
    add_subdirectory(python)
    target_link_libraries(putils PUBLIC puthon)
endif()

if (PUTILS_BUILD_PSE)
    add_subdirectory(pse)
    target_link_libraries(putils PUBLIC pse)
endif ()

if (PUTILS_NO_SHADER_DEBUG)
    target_compile_definitions(putils PUBLIC PUTILS_NO_SHADER_DEBUG)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
