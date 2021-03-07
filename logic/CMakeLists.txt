aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ SOURCE_LIB)
include_directories(include/)

add_library(logic STATIC ${SOURCE_LIB})
