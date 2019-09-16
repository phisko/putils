# SFML

set(SFML_ROOT ${CMAKE_CURRENT_LIST_DIR}/SFML)

set(BUILD_SHARED_LIBS TRUE)
add_subdirectory(${SFML_ROOT})

set(SFML_INCLUDE_DIR ${SFML_ROOT}/include)

#
# SFML linking for SFML Engine
#

set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)

set(TGUI_DIR TGUI-0.7.7)

if (TGUI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPSE_TGUI" PARENT_SCOPE)

    set(SFML_ROOT "${CMAKE_CURRENT_LIST_DIR}/SFML")
    add_subdirectory(${TGUI_DIR})
    FILE(COPY ${TGUI_DIR}/widgets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    FILE(COPY ${TGUI_DIR}/fonts DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    set(SFML_LIBRARIES tgui ${SFML_LIBRARIES})
endif ()

#
# SFML Engine
#

file(GLOB pseSrc *.cpp *.hpp)
add_library(pse STATIC ${pseSrc})
target_include_directories(pse PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${SFML_INCLUDE_DIR} ${TGUI_DIR}/include)
target_compile_definitions(pse PUBLIC PSE_TGUI)
target_link_libraries(pse putils ${SFML_LIBRARIES})
