aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ SOURCE_LIB)
include_directories(inc/)

add_library(logic STATIC ${SOURCE_LIB})
