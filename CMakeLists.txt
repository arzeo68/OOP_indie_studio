cmake_minimum_required(VERSION 3.11)
project(indie_studio)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB_RECURSE src "src/*.*pp")
add_executable(bomberman main.cpp ${src})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Irrlicht REQUIRED)
include_directories(${Irrlicht_INCLUDE_DIRS})

find_package(SFML 2 COMPONENTS system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

if (MSVC AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(INDIE_STUDIO_COMPILER "MSVC")
    set(INDIE_STUDIO_COMPILER_MSVC ON)
    target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_COMPILER_MSVC)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (MSVC)
        set(INDIE_STUDIO_COMPILER "Clang-cl")
        set(INDIE_STUDIO_COMPILER_CLANG_CL ON)
    else ()
        set(INDIE_STUDIO_COMPILER "Clang")
    endif ()

    set(INDIE_STUDIO_COMPILER_CLANG ON)
    target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_COMPILER_CLANG)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(INDIE_STUDIO_COMPILER "GCC")
    set(INDIE_STUDIO_COMPILER_GCC ON)
    target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_COMPILER_GCC)
endif ()

if (WIN32 OR CYGWIN)
    set(INDIE_STUDIO_PLATFORM "Windows")
    set(INDIE_STUDIO_PLATFORM_WINDOWS ON)
    target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_PLATFORM_WINDOWS)

    if (CYGWIN)
        set(INDIE_STUDIO_PLATFORM "${INDIE_STUDIO_PLATFORM} (Cygwin)")
        set(INDIE_STUDIO_PLATFORM_CYGWIN ON)
        target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_PLATFORM_CYGWIN)
    endif ()
    target_link_libraries(bomberman ${Irrlicht_LIBRARIES} ${SFML_LIBRARIES})
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/Irrlicht.dll"
            $<TARGET_FILE_DIR:bomberman>)
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/openal32.dll"
            $<TARGET_FILE_DIR:bomberman>)
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/sfml-system-2.dll"
            $<TARGET_FILE_DIR:bomberman>)
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/sfml-audio-2.dll"
            $<TARGET_FILE_DIR:bomberman>)
elseif (UNIX)
    set(INDIE_STUDIO_PLATFORM "Linux")
    set(INDIE_STUDIO_PLATFORM_LINUX ON)
    target_compile_definitions(bomberman PUBLIC INDIE_STUDIO_PLATFORM_LINUX)
    target_link_libraries(bomberman ${Irrlicht_LIBRARIES} ${SFML_LIBRARIES})
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/libirrlicht.so"
            $<TARGET_FILE_DIR:bomberman>)
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/libsfml-system.so"
            $<TARGET_FILE_DIR:bomberman>)
    add_custom_command(TARGET bomberman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/cmake/libsfml-audio.so"
            $<TARGET_FILE_DIR:bomberman>)
endif ()

add_custom_command(TARGET bomberman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/asset"
        $<TARGET_FILE_DIR:bomberman>/asset)
message(STATUS "Building for ${INDIE_STUDIO_PLATFORM} using ${INDIE_STUDIO_COMPILER} in ${CMAKE_BINARY_DIR}")