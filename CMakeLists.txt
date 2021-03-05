cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release." FORCE)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build" CACHE STRING "The install location." FORCE)
endif()

project(chess VERSION 1.0.1 LANGUAGES CXX DESCRIPTION "Chess game with GUI.")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE_EXE)
add_executable(${PROJECT_NAME} ${SOURCE_EXE})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESOURCES_DIR res/
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system logic)
target_include_directories(${PROJECT_NAME} PRIVATE inc SFML/include)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DRES=\"${RESOURCES_DIR}\"
)

add_compile_options(
    -pipe
    -Wall
    -Wextra
    -Wpedantic
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(
        -Ofast
        -march=native
        -fomit-frame-pointer
    )
    add_link_options(
        -flto
    )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(
        -O0
        -g3
        -fno-omit-frame-pointer
    )
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        add_compile_options(
            -fsanitize=address,undefined
        )
        add_link_options(
            -fsanitize=address,undefined
        )
    endif()
else()
    message("Unsupported build type: '" ${CMAKE_BUILD_TYPE}')
    return()
endif()

add_subdirectory(logic)

set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)

add_subdirectory(SFML)
