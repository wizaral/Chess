cmake_minimum_required(VERSION 3.18)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(resources_dir res/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # add_compile_definitions(
    #     /D GAME_RESOURCES_DIR="${resources_dir}"
    # )
else()
    add_compile_options(
        -pipe
        -Wall
        -Wextra
        -Wpedantic
        # -D GAME_RESOURCES_DIR "${resources_dir}"
    )
    # fix this for anouther systems
    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_compile_options(
            -Ofast
            -march=native
            -fomit-frame-pointer
            -flto
        )
    else()
        add_compile_options(
            -O0
            -g3
            -fno-omit-frame-pointer
            -fsanitize=address,undefined
        )
        add_link_options(
            -fsanitize=address,undefined
        )
    endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${resources_dir}")
    file(COPY graphics/${resources_dir} DESTINATION ${CMAKE_SOURCE_DIR}/${resources_dir})
endif()

set(BUILD_SHARED_LIBS FALSE)
set(SFML_USE_STATIC_STD_LIBS TRUE)

add_subdirectory(SFML)
add_subdirectory(logic)
add_subdirectory(graphics)

# cmake . -Bbuild -Thost=[x86|x64] -Wdev -Werror=dev && cmake --build ./build --config [Debug|Release]
