aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_EXE)
include_directories(../SFML/include/ ../logic/)

add_executable(chess ${SOURCE_EXE})
target_link_libraries(chess sfml-graphics sfml-window sfml-system lib-logic)
