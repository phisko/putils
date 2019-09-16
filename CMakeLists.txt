set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/CMakeModules)

file(GLOB src *.cpp *.hpp)
add_library(putils STATIC ${src})
target_include_directories(putils PUBLIC .)

add_subdirectory(pluginManager)
target_link_libraries(putils PUBLIC pluginManager)

set(imguiSrc imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_draw.cpp)
add_library(imgui STATIC ${imguiSrc})
target_include_directories(imgui PUBLIC imgui)
target_link_libraries(putils PUBLIC imgui)

if (PUTILS_BUILD_CONNECTION)
    add_subdirectory(connection)
    target_link_libraries(putils PUBLIC connection)
endif()

if(PUTILS_BUILD_LUA)
    add_subdirectory(lua)
    target_link_libraries(putils PUBLIC lua)
endif()

if(PUTILS_BUILD_MEDIATOR)
    add_subdirectory(mediator)
    target_link_libraries(putils PUBLIC mediator)
endif()

if(PUTILS_BUILD_HTTP)
    add_subdirectory(http)
    target_link_libraries(putils PUBLIC http)
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

add_subdirectory(glm)
target_link_libraries(glm_static)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
