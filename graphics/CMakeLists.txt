aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ SOURCE_EXE)
include_directories(../SFML/include/ ../logic/inc/ inc/)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system lib-logic)
